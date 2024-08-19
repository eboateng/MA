// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Telemetry_Fly_UnrealPawn.h"
#include "Telemetry_Fly_Unreal.h"

#include "IForceSeatMI.h"
#include "ForceSeatMIPlane.h"

ATelemetry_Fly_UnrealPawn::ATelemetry_Fly_UnrealPawn()
	: Controller(nullptr)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character
	SpringArm->SocketOffset = FVector(0.f,0.f,60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	RollSpeed = 400.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
}

ATelemetry_Fly_UnrealPawn::~ATelemetry_Fly_UnrealPawn()
{
	// ForceSeatMI - BEGIN
	delete Controller;
	Controller = nullptr;
	// ForceSeatMI - END
}

void ATelemetry_Fly_UnrealPawn::Tick(float DeltaSeconds)
{
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0,0,0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);

	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	// ForceSeatMI - BEGIN

	++Iterator;

	// Use extra parameters to generate custom effects, for exmp. vibrations. They will NOT be
	// filtered, smoothed or processed in any way.
	ExtraTransformation.yaw     = 0;
	ExtraTransformation.pitch   = 0;
	ExtraTransformation.roll    = 0;
	// TODO: Uncomment to enable vibrations:
	// Parameters.roll  = sinf(Iterator) * 0.05f;
	ExtraTransformation.right   = 0;
	ExtraTransformation.up      = 0;
	ExtraTransformation.forward = 0;

	if (Controller)
	{
		Controller->AddExtraTransformation(ExtraTransformation);
		Controller->Update(DeltaSeconds);
	}
	// ForceSeatMI - END
}

void ATelemetry_Fly_UnrealPawn::BeginPlay()
{
	Super::BeginPlay();

	Iterator = 0;

	// ForceSeatMI - BEGIN
	delete Controller;

	Controller = ForceSeatMIPlane::Get().CreateControllerCore(this);

	if (Controller)
	{
		Controller->Pause(false);
		Controller->Begin();
	}
	// ForceSeatMI - END
}

void ATelemetry_Fly_UnrealPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// ForceSeatMI - BEGIN
	if (Controller)
	{
		Controller->End();
	}
	// ForceSeatMI - END
}

void ATelemetry_Fly_UnrealPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void ATelemetry_Fly_UnrealPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &ATelemetry_Fly_UnrealPawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ATelemetry_Fly_UnrealPawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATelemetry_Fly_UnrealPawn::MoveRightInput);
	PlayerInputComponent->BindAxis("Rotate", this, &ATelemetry_Fly_UnrealPawn::RotateInput);
}

void ATelemetry_Fly_UnrealPawn::ThrustInput(float Val)
{
	// Is there no input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ATelemetry_Fly_UnrealPawn::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ATelemetry_Fly_UnrealPawn::MoveRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}
void ATelemetry_Fly_UnrealPawn::RotateInput(float Val)
{
	float TargetRollSpeed = (Val * RollSpeed);
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

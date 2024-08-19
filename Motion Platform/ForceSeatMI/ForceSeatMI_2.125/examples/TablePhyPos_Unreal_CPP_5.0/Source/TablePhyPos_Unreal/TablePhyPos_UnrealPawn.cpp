/*
 * Copyright (C) 2012-2022 MotionSystems
 *
 * This file is part of ForceSeatMI SDK.
 *
 * www.motionsystems.eu
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "TablePhyPos_UnrealPawn.h"
#include "TablePhyPos_Unreal.h"
#include "IForceSeatMI.h"

namespace
{
	// Maximum speed that platform can move with
	const int PLATFORM_MAX_SPEED = 25000;

	// Heave maximum value that is available in the game
	const float DRAWING_HEAVE_MAX = 80.f;

	// Maximum value of pitch angle that is available in the game
	const float DRAWING_PITCH_MAX = FMath::DegreesToRadians(12);

	// Maximum value of roll angle that is available in the game
	const float DRAWING_ROLL_MAX = FMath::DegreesToRadians(12);
}

ATablePhyPos_UnrealPawn::ATablePhyPos_UnrealPawn()
	: CurrentDrawingHeave(0)
	, CurrentDrawingPitch(0)
	, CurrentDrawingRoll(0)
	, Api(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Create platform's board subobject
	Board = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board0"));
	Board->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Board->SetSimulatePhysics(false);
	Board->SetNotifyRigidBodyCollision(true);

	// Create platform's base subobject
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base0"));
	Base->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Base->SetSimulatePhysics(false);
	Base->SetNotifyRigidBodyCollision(true);

	// Create platform's shaft subobject
	Shaft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shaft0"));
	Shaft->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Shaft->SetSimulatePhysics(false);
	Shaft->SetNotifyRigidBodyCollision(true);

	// Create shaft's head subobject
	ShaftHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShaftHead0"));
	ShaftHead->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	ShaftHead->SetSimulatePhysics(false);
	ShaftHead->SetNotifyRigidBodyCollision(true);

	// Create root subobject
	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root0"));
	Root->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Root->SetSimulatePhysics(false);
	Root->SetNotifyRigidBodyCollision(true);

	RootComponent = Root;

	// ForceSeatMI - BEGIN
	// Initialize platform position structure
	memset(&PlatformPosition, 0, sizeof(PlatformPosition));
	PlatformPosition.structSize = sizeof(PlatformPosition);

	// The demo program is able to provide pause, position and speed limit
	PlatformPosition.maxSpeed = PLATFORM_MAX_SPEED;
	PlatformPosition.mask = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;
	// ForceSeatMI - END
}

ATablePhyPos_UnrealPawn::~ATablePhyPos_UnrealPawn()
{
	delete Api;
	Api = nullptr;
}

void ATablePhyPos_UnrealPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentDrawingRoll  = DRAWING_ROLL_MAX  * FMath::Sin(3.0f * GetGameTimeSinceCreation());
	CurrentDrawingPitch = DRAWING_PITCH_MAX * FMath::Sin(1.5f * GetGameTimeSinceCreation());
	CurrentDrawingHeave = DRAWING_HEAVE_MAX * FMath::Sin(0.7f * GetGameTimeSinceCreation());

	// Update board's origin transformation (in game) with current heave, pitch and roll values
	FVector BoardLocation = BoardOriginTransform.GetLocation();
	FQuat BoardRotation = BoardOriginTransform.GetRotation();
	BoardLocation.Z += CurrentDrawingHeave;
	BoardRotation.X += CurrentDrawingPitch;
	BoardRotation.Y += CurrentDrawingRoll;

	// Set calculated transformation for board subobject
	Board->SetRelativeLocationAndRotation(BoardLocation, BoardRotation);

	// Update shaft's origin transformation  (in game) with current heave value
	FVector ShaftLocation = ShaftOriginTransform.GetLocation();
	ShaftLocation.Z += CurrentDrawingHeave;

	// Set calculated transformation for shaft subobject
	Shaft->SetRelativeLocationAndRotation(ShaftLocation, ShaftOriginTransform.GetRotation());

	// Update shaft's head origin transformation  (in game) with current heave value
	FVector ShaftHeadLocation = ShaftHeadOriginTransform.GetLocation();
	ShaftHeadLocation.Z += CurrentDrawingHeave;

	// Set calculated transformation for shaft's head subobject
	ShaftHead->SetRelativeLocationAndRotation(ShaftHeadLocation, ShaftHeadOriginTransform.GetRotation());

	// ForceSeatMI - BEGIN
	SendCoordinatesToPlatform();
	// ForceSeatMI - END
}

void ATablePhyPos_UnrealPawn::BeginPlay()
{
	Super::BeginPlay();

	// ForceSeatMI - BEGIN
	delete Api;

	Api = IForceSeatMI::Get().CreateAPI("SDK - Positioning");

	if (Api)
	{
		Api->BeginMotionControl();
	}

	SendCoordinatesToPlatform();
	// ForceSeatMI - END

	// Get origin transformations for movable subobjects
	BoardOriginTransform = Board->GetRelativeTransform();
	ShaftOriginTransform = Shaft->GetRelativeTransform();
	ShaftHeadOriginTransform = ShaftHead->GetRelativeTransform();
}

void ATablePhyPos_UnrealPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// ForceSeatMI - BEGIN
	if (Api)
	{
		Api->EndMotionControl();
	}
	// ForceSeatMI - END
}

void ATablePhyPos_UnrealPawn::SendCoordinatesToPlatform()
{
	// ForceSeatMI - BEGIN
	if (Api)
	{
		PlatformPosition.state =  FSMI_STATE_NO_PAUSE;
		PlatformPosition.roll  =  CurrentDrawingRoll;
		PlatformPosition.pitch = -CurrentDrawingPitch;
		PlatformPosition.heave =  CurrentDrawingHeave;

		Api->SendTopTablePosPhy(&PlatformPosition);
	}
	// ForceSeatMI - END
}

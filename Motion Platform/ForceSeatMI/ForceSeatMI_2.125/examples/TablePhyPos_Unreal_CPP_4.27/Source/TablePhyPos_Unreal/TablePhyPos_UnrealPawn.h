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
 
#pragma once

#include "ForceSeatMI_Positioning.h"
#include "IForceSeatMI_API.h"
#include "GameFramework/Pawn.h"
#include "TablePhyPos_UnrealPawn.generated.h"

UCLASS()
class TABLEPHYPOS_UNREAL_API ATablePhyPos_UnrealPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATablePhyPos_UnrealPawn();
	virtual ~ATablePhyPos_UnrealPawn();

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Returns mesh subobject that represents platform's board */
	FORCEINLINE class UStaticMeshComponent* GetBoard() const { return Board; }

	/** Returns mesh subobject that represents base of the platform */
	FORCEINLINE class UStaticMeshComponent* GetBase() const { return Base; }

	/** Returns mesh subobject that represents platform's shaft */
	FORCEINLINE class UStaticMeshComponent* GetShaft() const { return Shaft; }

	/** Returns mesh subobject that represents shaft's head */
	FORCEINLINE class UStaticMeshComponent* GetShaftHead() const { return ShaftHead; }

	/** Returns root subobject */
	FORCEINLINE class UStaticMeshComponent* GetRoot() const { return Root; }

protected:
	/** Called when play begins */
	virtual void BeginPlay() override;

	/** Called whenever actor is removed from level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** Sends logical coordinates to the platform */
	void SendCoordinatesToPlatform();

private:
	/** Platform's board component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Board;

	/** Platform's base component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Base;

	/** Platform's shaft component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Shaft;

	/** Shaft's head component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShaftHead;

	/** Root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Root;

	/** Current platform's heave in game */
	float CurrentDrawingHeave;

	/** Current platform's pitch in game */
	float CurrentDrawingPitch;

	/** Current platform's roll in game */
	float CurrentDrawingRoll;

	/** Transformation that represents origin position of platform's board component in game coordinates */
	FTransform BoardOriginTransform;

	/** Transformation that represents origin position of platform's shaft component in game coordinates */
	FTransform ShaftOriginTransform;

	/** Transformation that represents origin position of shaft's head component in game coordinates */
	FTransform ShaftHeadOriginTransform;

	/** Position in physical coordinates that will be send to the platform */
	FSMI_TopTablePositionPhysical PlatformPosition;

	/** Object used to drive motion platform */
	IForceSeatMI_API* Api;
};

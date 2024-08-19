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

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WheeledVehiclePawn.h"

#include "IForceSeatMI_API.h"
#include "ForceSeatMI_TelemetryACE.h"

#include "ForceSeatMIChaosVehicle_Controller.generated.h"

UCLASS( ClassGroup=(ForceSeatMIChaos), meta=(BlueprintSpawnableComponent) )
class UForceSeatMIChaosVehicle_Controller : public UActorComponent
{
	GENERATED_BODY()
public:
	UForceSeatMIChaosVehicle_Controller();
	virtual ~UForceSeatMIChaosVehicle_Controller();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ForceSeatMIChaosVehicle")
	void SetVehicleObject(AWheeledVehiclePawn* Input);

	void Pause(bool paused);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	AWheeledVehiclePawn* m_vehicle;
	IForceSeatMI_API*    m_api;
	bool                 m_firstCall;
	FSMI_TelemetryACE    m_telemetry;
	FTransform           m_prevTransform;
	float                m_prevSurgeSpeed;
	float                m_prevSwaySpeed;
	float                m_prevHeaveSpeed;
};

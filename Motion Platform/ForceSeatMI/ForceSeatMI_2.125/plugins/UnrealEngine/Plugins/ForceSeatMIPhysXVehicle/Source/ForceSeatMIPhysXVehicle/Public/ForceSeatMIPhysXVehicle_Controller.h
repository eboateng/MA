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

// Disable PhysX deprecation related warnings (check ChaosVehiclePhysics Plugin)
#pragma warning(disable : 4996)

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WheeledVehicle.h"

#include "ForceSeatMIPhysXVehicle_ControllerCore.h"

#include "ForceSeatMIPhysXVehicle_Controller.generated.h"

UCLASS( ClassGroup=(ForceSeatMIPhysXVehicle), meta=(BlueprintSpawnableComponent) )
class UForceSeatMIPhysXVehicle_Controller : public UActorComponent
{
	GENERATED_BODY()
public:
	UForceSeatMIPhysXVehicle_Controller();
	virtual ~UForceSeatMIPhysXVehicle_Controller();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ForceSeatMIPhysXVehicle")
	void SetVehicleObject(AWheeledVehicle* Input);

	void Pause(bool paused);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	ForceSeatMIPhysXVehicle_ControllerCore* m_controller;
	AWheeledVehicle*                        m_source;
};

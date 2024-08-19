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

#include "ForceSeatMIPlane_ControllerCore.h"

#include "ForceSeatMIPlane_Controller.generated.h"

UCLASS( ClassGroup=(ForceSeatMIPlane), meta=(BlueprintSpawnableComponent) )
class UForceSeatMIPlane_Controller : public UActorComponent
{
	GENERATED_BODY()
public:
	UForceSeatMIPlane_Controller();
	virtual ~UForceSeatMIPlane_Controller();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ForceSeatMIPlane")
	void SetPlaneObject(APawn* Input);

	void Pause(bool paused);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	IForceSeatMIPlane_ControllerCore* m_controller;
	APawn*                            m_source;
};

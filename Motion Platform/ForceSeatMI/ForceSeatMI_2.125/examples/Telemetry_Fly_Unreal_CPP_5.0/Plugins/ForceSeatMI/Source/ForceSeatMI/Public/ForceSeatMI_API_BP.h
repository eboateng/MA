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

#include "IForceSeatMI_API.h"
#include "ForceSeatMI_UStructs.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ForceSeatMI_API_BP.generated.h"

UCLASS(ClassGroup = (ForceSeatMI), meta = (BlueprintSpawnableComponent))
class UForceSeatMI_API_BP : public UActorComponent
{
	GENERATED_BODY()
public:
	UForceSeatMI_API_BP();
	virtual ~UForceSeatMI_API_BP();

public:
	UFUNCTION(BlueprintCallable, Category = "ForceSeatMI")
	void Update(const FTopTablePositionPhysicalBP& PhysicalPosition);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	IForceSeatMI_API*             Api;
	FSMI_TopTablePositionPhysical Position;
};

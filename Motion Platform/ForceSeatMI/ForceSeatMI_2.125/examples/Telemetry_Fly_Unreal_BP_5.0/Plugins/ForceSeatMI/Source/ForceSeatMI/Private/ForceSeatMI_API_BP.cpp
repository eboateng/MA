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

#include "ForceSeatMI_API_BP.h"
#include "ForceSeatMI_Positioning.h"
#include "IForceSeatMI.h"

#include "Math/UnrealMathUtility.h"

UForceSeatMI_API_BP::UForceSeatMI_API_BP()
	: Api(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;

	memset(&Position, 0, sizeof(Position));
	Position.structSize = sizeof(Position);
	Position.mask       = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;
}

UForceSeatMI_API_BP::~UForceSeatMI_API_BP()
{
	delete Api;
	Api = nullptr;
}

void UForceSeatMI_API_BP::Update(const FTopTablePositionPhysicalBP& PhysicalPosition)
{
	Position.pitch    = PhysicalPosition.Pitch;
	Position.roll     = PhysicalPosition.Roll;
	Position.yaw      = PhysicalPosition.Yaw;
	Position.sway     = PhysicalPosition.Sway;
	Position.surge    = PhysicalPosition.Surge;
	Position.heave    = PhysicalPosition.Heave;
	Position.state    = PhysicalPosition.Paused ? FSMI_STATE_PAUSE : FSMI_STATE_NO_PAUSE;
	Position.maxSpeed = FMath::Clamp(PhysicalPosition.MaxSpeed, 0, 65535);
}

void UForceSeatMI_API_BP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Api)
	{
		Api->SendTopTablePosPhy(&Position);
	}
}

void UForceSeatMI_API_BP::BeginPlay()
{
	Super::BeginPlay();

	delete Api;

	Api = IForceSeatMI::Get().CreateAPI();

	if (Api)
	{
		Api->BeginMotionControl();
	}
}

void UForceSeatMI_API_BP::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Api)
	{
		Api->EndMotionControl();
	}
}

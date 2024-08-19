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

#include "ForceSeatMI_CarSimObserver.h"
#include "IForceSeatMI.h"
#include "ForceSeatMI_CarSim.h"

AForceSeatMI_CarSimObserver::AForceSeatMI_CarSimObserver()
{
	PrimaryActorTick.bCanEverTick = true;
	AccLowPasFactor               = 0.7f;
	AngleSpeedLowPasFactor        = 0.7f;
}

void AForceSeatMI_CarSimObserver::BeginPlay()
{
	if (CarSimPawn)
	{
		// ForceSeatMI - BEGIN
		// Change this if you are going to use dedicated game profile
		IForceSeatMI::Get().ActivateProfile("SDK - Vehicle Telemetry ACE");
		IForceSeatMI::Get().SetTelemetryObject(new ForceSeatMI_CarSim(*CarSimPawn, AccLowPasFactor, AngleSpeedLowPasFactor));
		IForceSeatMI::Get().Pause(false);
		IForceSeatMI::Get().Begin();
		// ForceSeatMI - END
	}

	Super::BeginPlay();
}

void AForceSeatMI_CarSimObserver::Tick(float DeltaTime)
{
	// ForceSeatMI - BEGIN
	IForceSeatMI::Get().Update(DeltaTime);
	// ForceSeatMI - END

	Super::Tick(DeltaTime);
}

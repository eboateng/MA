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

#include "ForceSeatMI_Status.h"
#include "ForceSeatMI_Positioning.h"
#include "ForceSeatMI_Telemetry.h"
#include "ForceSeatMI_TelemetryACE.h"
#include "ForceSeatMI_TactileTransducers.h"
#include "ForceSeatMI_SFX.h"

class IForceSeatMI_API
{
public:
	virtual ~IForceSeatMI_API() { }
	virtual bool BeginMotionControl        () = 0;
	virtual bool EndMotionControl          () = 0;
	virtual bool GetPlatformInfoEx         (FSMI_PlatformInfo* platformInfo, unsigned int platformInfoStructSize, unsigned int timeout) = 0;
	virtual bool SendTelemetry             (const FSMI_Telemetry* telemetry) = 0;
	virtual bool SendTelemetry2            (const FSMI_Telemetry* telemetry, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) = 0;
	virtual bool SendTelemetryACE          (const FSMI_TelemetryACE* telemetry) = 0;
	virtual bool SendTelemetryACE2         (const FSMI_TelemetryACE* telemetry, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) = 0;
	virtual bool SendTopTablePosLog        (const FSMI_TopTablePositionLogical* position) = 0;
	virtual bool SendTopTablePosLog2       (const FSMI_TopTablePositionLogical* position, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) = 0;
	virtual bool SendTopTablePosPhy        (const FSMI_TopTablePositionPhysical* position) = 0;
	virtual bool SendTopTablePosPhy2       (const FSMI_TopTablePositionPhysical* position, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) = 0;
	virtual bool SendTopTableMatrixPhy     (const FSMI_TopTableMatrixPhysical* matrix) = 0;
	virtual bool SendTopTableMatrixPhy2    (const FSMI_TopTableMatrixPhysical* matrix, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) = 0;
	virtual bool SendTactileFeedbackEffects(const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) = 0;
};

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
#include "ForceSeatMI_Functions.h"

class ForceSeatMI_API : public IForceSeatMI_API
{
public:
	explicit ForceSeatMI_API(FSMI_Handle handle);

public:
	virtual bool BeginMotionControl        () override;
	virtual bool EndMotionControl          () override;
	virtual bool GetPlatformInfoEx         (FSMI_PlatformInfo* platformInfo, unsigned int platformInfoStructSize, unsigned int timeout) override;
	virtual bool SendTelemetry             (const FSMI_Telemetry* telemetry) override;
	virtual bool SendTelemetry2            (const FSMI_Telemetry* telemetry, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) override;
	virtual bool SendTelemetryACE          (const FSMI_TelemetryACE* telemetry) override;
	virtual bool SendTelemetryACE2         (const FSMI_TelemetryACE* telemetry, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) override;
	virtual bool SendTopTablePosLog        (const FSMI_TopTablePositionLogical* position) override;
	virtual bool SendTopTablePosLog2       (const FSMI_TopTablePositionLogical* position, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) override;
	virtual bool SendTopTablePosPhy        (const FSMI_TopTablePositionPhysical* position) override;
	virtual bool SendTopTablePosPhy2       (const FSMI_TopTablePositionPhysical* position, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) override;
	virtual bool SendTopTableMatrixPhy     (const FSMI_TopTableMatrixPhysical* matrix) override;
	virtual bool SendTopTableMatrixPhy2    (const FSMI_TopTableMatrixPhysical* matrix, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) override;
	virtual bool SendTactileFeedbackEffects(const FSMI_TactileAudioBasedFeedbackEffects* audioEffects) override;

private:
	FSMI_Handle m_handle;
};

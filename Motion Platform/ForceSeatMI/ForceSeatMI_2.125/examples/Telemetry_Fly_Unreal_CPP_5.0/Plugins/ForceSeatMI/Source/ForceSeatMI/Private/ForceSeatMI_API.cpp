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

#include "ForceSeatMI_API.h"

ForceSeatMI_API::ForceSeatMI_API(FSMI_Handle handle)
	: m_handle(handle)
{
}

bool ForceSeatMI_API::BeginMotionControl()
{
	return ForceSeatMI_BeginMotionControl(m_handle) != FSMI_False;
}

bool ForceSeatMI_API::EndMotionControl()
{
	return ForceSeatMI_EndMotionControl(m_handle) != FSMI_False;
}

bool ForceSeatMI_API::GetPlatformInfoEx(FSMI_PlatformInfo* platformInfo, unsigned int platformInfoStructSize, unsigned int timeout)
{
	return ForceSeatMI_GetPlatformInfoEx(m_handle, platformInfo, platformInfoStructSize, timeout) != FSMI_False;
}

bool ForceSeatMI_API::SendTelemetry(const FSMI_Telemetry* telemetry)
{
	return ForceSeatMI_SendTelemetry(m_handle, telemetry) != FSMI_False;
}

bool ForceSeatMI_API::SendTelemetry2(const FSMI_Telemetry* telemetry, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	return ForceSeatMI_SendTelemetry2(m_handle, telemetry, sfx, audioEffects) != FSMI_False;
}

bool ForceSeatMI_API::SendTelemetryACE(const FSMI_TelemetryACE* telemetry)
{
	return ForceSeatMI_SendTelemetryACE(m_handle, telemetry) != FSMI_False;
}

bool ForceSeatMI_API::SendTelemetryACE2(const FSMI_TelemetryACE* telemetry, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	return ForceSeatMI_SendTelemetryACE2(m_handle, telemetry, sfx, audioEffects) != FSMI_False;
}

bool ForceSeatMI_API::SendTopTablePosLog(const FSMI_TopTablePositionLogical* position)
{
	return ForceSeatMI_SendTopTablePosLog(m_handle, position) != FSMI_False;
}

bool ForceSeatMI_API::SendTopTablePosLog2(const FSMI_TopTablePositionLogical* position, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	return ForceSeatMI_SendTopTablePosLog2(m_handle, position, sfx, audioEffects) != FSMI_False;
}

bool ForceSeatMI_API::SendTopTablePosPhy(const FSMI_TopTablePositionPhysical* position)
{
	return ForceSeatMI_SendTopTablePosPhy(m_handle, position) != FSMI_False;
}

bool ForceSeatMI_API::SendTopTablePosPhy2(const FSMI_TopTablePositionPhysical* position, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	return ForceSeatMI_SendTopTablePosPhy2(m_handle, position, sfx, audioEffects) != FSMI_False;
}

bool ForceSeatMI_API::SendTopTableMatrixPhy(const FSMI_TopTableMatrixPhysical* matrix)
{
	return ForceSeatMI_SendTopTableMatrixPhy(m_handle, matrix) != FSMI_False;
}

bool ForceSeatMI_API::SendTopTableMatrixPhy2(const FSMI_TopTableMatrixPhysical* matrix, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	return ForceSeatMI_SendTopTableMatrixPhy2(m_handle, matrix, sfx, audioEffects) != FSMI_False;
}

bool ForceSeatMI_API::SendTactileFeedbackEffects(const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	return ForceSeatMI_SendTactileFeedbackEffects(m_handle, audioEffects) != FSMI_False;
}

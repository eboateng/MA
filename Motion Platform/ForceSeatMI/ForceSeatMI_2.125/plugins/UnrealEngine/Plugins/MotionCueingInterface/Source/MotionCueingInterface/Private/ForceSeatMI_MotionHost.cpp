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

#include "ForceSeatMI_MotionHost.h"

#include "IForceSeatMI.h"
#include "IForceSeatMI_API.h"

namespace
{
	constexpr const float DEG_TO_RAD = 0.0174532925f;
	constexpr const float CM_TO_M    = 0.01f;
}

ForceSeatMI_MotionHost::ForceSeatMI_MotionHost(const char* profile, const char* appID)
	: m_api(nullptr)
{
	m_api = IForceSeatMI::Get().CreateAPI(profile, appID);

	memset(&m_telemetry, 0, sizeof(m_telemetry));
	m_telemetry.structSize = sizeof(m_telemetry);
}

ForceSeatMI_MotionHost::~ForceSeatMI_MotionHost()
{
	delete m_api;
	m_api = nullptr;
}

void ForceSeatMI_MotionHost::Initialize()
{
	m_api->BeginMotionControl();
}

void ForceSeatMI_MotionHost::Cleanup()
{
	m_api->EndMotionControl();
}

void ForceSeatMI_MotionHost::ConnectToController(FString ControllerIP, int ControllerPort, int HostRcvPort)
{
	(void)ControllerIP;
	(void)ControllerPort;
	(void)HostRcvPort;
}

void ForceSeatMI_MotionHost::ReceiveDataFromController()
{
}

void ForceSeatMI_MotionHost::SetStateFlow()
{
}

void ForceSeatMI_MotionHost::SendDataToController(FMotionData& Data)
{
	MapData(Data);

	m_api->SendTelemetryACE(&m_telemetry);
}

void ForceSeatMI_MotionHost::MapData(FMotionData& Data)
{
	m_telemetry.bodyAngularVelocity.pitch = Data.PitchVelocity * DEG_TO_RAD;
	m_telemetry.bodyAngularVelocity.roll  = Data.RollVelocity  * DEG_TO_RAD;
	m_telemetry.bodyAngularVelocity.yaw   = Data.YawVelocity   * DEG_TO_RAD;

	m_telemetry.bodyLinearAcceleration.forward = Data.AccelerationX * CM_TO_M;
	m_telemetry.bodyLinearAcceleration.right   = Data.AccelerationY * CM_TO_M;
	m_telemetry.bodyLinearAcceleration.upward  = Data.AccelerationZ * CM_TO_M;
}

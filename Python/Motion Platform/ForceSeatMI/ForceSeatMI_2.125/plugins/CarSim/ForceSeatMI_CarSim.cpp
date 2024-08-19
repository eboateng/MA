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

#include "ForceSeatMI_CarSim.h"
#include "ForceSeatMI_Helpers.h"

#include "CarsimPawn.h"

#include "CoreMinimal.h"

ForceSeatMI_CarSim::ForceSeatMI_CarSim(const ACarsimPawn& car, float accLPF, float angleSpeedLPF)
	: m_car(car)
	, m_firstCall(true)
	, m_accLPF(accLPF)
	, m_angleSpeedLPF(angleSpeedLPF)
{
	memset(&m_telemetry, 0, sizeof(m_telemetry));
	m_telemetry.structSize = sizeof(m_telemetry);

	InitializeHandles();
}

void ForceSeatMI_CarSim::setAccelerationLowPassFactor(float factor)
{
	m_accLPF = factor;
}

void ForceSeatMI_CarSim::setAngleSpeedLowPassFactor(float factor)
{
	m_angleSpeedLPF = factor;
}

void ForceSeatMI_CarSim::Begin()
{
	m_firstCall = true;
}

void ForceSeatMI_CarSim::End()
{
	m_firstCall = true;
}

void ForceSeatMI_CarSim::Update(float deltaTime)
{
	auto movement = m_car.GetCarSimMovement();

	if (movement)
	{
		m_telemetry.rpm        = movement->GetFloatValue_Implementation(m_rpmHandle);
		m_telemetry.maxRpm     = 0; //todo
		m_telemetry.gearNumber = movement->GetFloatValue_Implementation(m_gearNumberHandle);
		m_telemetry.speed      = movement->GetFloatValue_Implementation(m_surgeSpeedHandle);
		m_telemetry.surgeSpeed = movement->GetFloatValue_Implementation(m_surgeSpeedHandle);
		m_telemetry.swaySpeed  = movement->GetFloatValue_Implementation(m_swaySpeedHandle);
		m_telemetry.heaveSpeed = movement->GetFloatValue_Implementation(m_heaveSpeedHandle);
		m_telemetry.pitch      = movement->GetFloatValue_Implementation(m_pitchHandle);
		m_telemetry.roll       = movement->GetFloatValue_Implementation(m_rollHandle);
		m_telemetry.yaw        = movement->GetFloatValue_Implementation(m_yawHandle);

		if (m_firstCall)
		{
			m_firstCall = false;

			m_telemetry.surgeAcceleration = 0;
			m_telemetry.swayAcceleration  = 0;
			m_telemetry.heaveAcceleration = 0;
			m_telemetry.pitchSpeed        = 0;
			m_telemetry.rollSpeed         = 0;
			m_telemetry.yawSpeed          = 0;
		}
		else
		{
			ForceSeatMI_Helpers::lowPass<float>(m_telemetry.surgeAcceleration, movement->GetFloatValue_Implementation(m_surgeAccHandle), m_accLPF);
			ForceSeatMI_Helpers::lowPass<float>(m_telemetry.swayAcceleration,  movement->GetFloatValue_Implementation(m_swayAccHandle),  m_accLPF);
			ForceSeatMI_Helpers::lowPass<float>(m_telemetry.heaveAcceleration, movement->GetFloatValue_Implementation(m_heaveAccHandle), m_accLPF);

			ForceSeatMI_Helpers::lowPass<float>(m_telemetry.pitchSpeed, movement->GetFloatValue_Implementation(m_pitchSpeedHandle), m_angleSpeedLPF);
			ForceSeatMI_Helpers::lowPass<float>(m_telemetry.rollSpeed,  movement->GetFloatValue_Implementation(m_rollSpeedHandle),  m_angleSpeedLPF);
			ForceSeatMI_Helpers::lowPass<float>(m_telemetry.yawSpeed,   movement->GetFloatValue_Implementation(m_yawSpeedHandle),   m_angleSpeedLPF);
		}
	}

	DebugValue(L"RPM:         ", m_telemetry.rpm);
	DebugValue(L"Max RPM:     ", m_telemetry.maxRpm);
	DebugValue(L"Gear number: ", m_telemetry.gearNumber);
	DebugValue(L"Surge speed: ", m_telemetry.surgeSpeed);
	DebugValue(L"Sway speed:  ", m_telemetry.swaySpeed);
	DebugValue(L"Heave speed: ", m_telemetry.heaveSpeed);
	DebugValue(L"Surge acc:   ", m_telemetry.surgeAcceleration);
	DebugValue(L"Sway acc:    ", m_telemetry.swayAcceleration);
	DebugValue(L"Heave acc:   ", m_telemetry.heaveAcceleration);
	DebugValue(L"Pitch:       ", m_telemetry.pitch);
	DebugValue(L"Roll:        ", m_telemetry.roll);
	DebugValue(L"Yaw:         ", m_telemetry.yaw);
	DebugValue(L"Pitch speed: ", m_telemetry.pitchSpeed);
	DebugValue(L"Roll speed:  ", m_telemetry.rollSpeed);
	DebugValue(L"Yaw speed:   ", m_telemetry.yawSpeed);
}

void ForceSeatMI_CarSim::Pause(bool paused)
{
	m_firstCall = true;

	m_telemetry.state = paused ? FSMI_STATE_PAUSE : FSMI_STATE_NO_PAUSE;
}

const FSMI_TelemetryACE& ForceSeatMI_CarSim::GetTelemetry() const
{
	return m_telemetry;
}

void ForceSeatMI_CarSim::InitializeHandles()
{
	auto movement = m_car.GetCarSimMovement();

	if (movement)
	{
		m_rpmHandle        = movement->GetOutputHandle_Implementation(TEXT("AV_ENG"));
		m_gearNumberHandle = movement->GetOutputHandle_Implementation(TEXT("GEARSTAT"));

		m_surgeSpeedHandle = movement->GetOutputHandle_Implementation(TEXT("VX"));
		m_swaySpeedHandle  = movement->GetOutputHandle_Implementation(TEXT("VY"));
		m_heaveSpeedHandle = movement->GetOutputHandle_Implementation(TEXT("VZ"));
		m_surgeAccHandle   = movement->GetOutputHandle_Implementation(TEXT("AX"));
		m_swayAccHandle    = movement->GetOutputHandle_Implementation(TEXT("AY"));
		m_heaveAccHandle   = movement->GetOutputHandle_Implementation(TEXT("AZ"));

		m_pitchHandle      = movement->GetOutputHandle_Implementation(TEXT("PITCH"));
		m_rollHandle       = movement->GetOutputHandle_Implementation(TEXT("ROLL"));
		m_yawHandle        = movement->GetOutputHandle_Implementation(TEXT("YAW"));
		m_pitchSpeedHandle = movement->GetOutputHandle_Implementation(TEXT("AVY"));
		m_rollSpeedHandle  = movement->GetOutputHandle_Implementation(TEXT("AVX"));
		m_yawSpeedHandle   = movement->GetOutputHandle_Implementation(TEXT("AVZ"));
	}
}

void ForceSeatMI_CarSim::DebugValue(const wchar_t* name, float value)
{
	FString dbgMsg;

	dbgMsg  = name;
	dbgMsg += FString::SanitizeFloat(value);

	GEngine->AddOnScreenDebugMessage(-1, 0.001, FColor::Red, dbgMsg);
}

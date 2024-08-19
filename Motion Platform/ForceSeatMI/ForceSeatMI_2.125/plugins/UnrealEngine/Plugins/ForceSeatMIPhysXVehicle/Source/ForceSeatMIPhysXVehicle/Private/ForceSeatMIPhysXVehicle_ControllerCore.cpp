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

#include "ForceSeatMIPhysXVehicle_ControllerCore.h"
#include "IForceSeatMI.h"

#include "WheeledVehicleMovementComponent.h"
#include "WheeledVehicle.h"

namespace
{
	template<class T>
	inline T toMeters(T centimeters)
	{
		return centimeters / 100;
	}

	template <class T>
	inline void lowPass(T& stored, T newValue, T factor)
	{
		stored += (newValue - stored) * factor;
	}

	constexpr const float FSMI_VT_ACC_LOW_PASS_FACTOR          = 0.7f;
	constexpr const float FSMI_VT_ANGLES_SPEED_LOW_PASS_FACTOR = 0.7f;
}

ForceSeatMIPhysXVehicle_ControllerCore::ForceSeatMIPhysXVehicle_ControllerCore(AWheeledVehicle* vehicle)
	: m_vehicle(vehicle)
	, m_api(IForceSeatMI::Get().CreateAPI("SDK - Vehicle Telemetry ACE"))
	, m_firstCall(true)
{
	memset(&m_telemetry, 0, sizeof(m_telemetry));
	m_telemetry.structSize = sizeof(m_telemetry);
}

ForceSeatMIPhysXVehicle_ControllerCore::~ForceSeatMIPhysXVehicle_ControllerCore()
{
	delete m_api;
	m_api = nullptr;
}

void ForceSeatMIPhysXVehicle_ControllerCore::Update(float deltaTime)
{
	if (!m_vehicle)
	{
		return;
	}

	const auto  velocity         = m_vehicle->GetVelocity();
	const auto* vehicleMovement  = m_vehicle->GetVehicleMovement();
	const auto& currentTransform = m_vehicle->GetActorTransform();

	m_telemetry.rpm                 = vehicleMovement->GetEngineRotationSpeed();
	m_telemetry.maxRpm              = vehicleMovement->GetEngineMaxRotationSpeed();
	m_telemetry.gearNumber          = vehicleMovement->GetCurrentGear();
	m_telemetry.vehicleForwardSpeed = toMeters(vehicleMovement->GetForwardSpeed());

	auto forwardSpeed = toMeters(FVector::DotProduct(m_vehicle->GetActorForwardVector(), velocity));
	auto rightSpeed   = toMeters(FVector::DotProduct(m_vehicle->GetActorRightVector(), velocity));
	auto upSpeed      = toMeters(FVector::DotProduct(m_vehicle->GetActorUpVector(), velocity));

	if (m_firstCall)
	{
		m_firstCall = false;

		m_telemetry.bodyLinearAcceleration.forward = 0;
		m_telemetry.bodyLinearAcceleration.right   = 0;
		m_telemetry.bodyLinearAcceleration.upward  = 0;
		m_telemetry.bodyAngularVelocity.pitch      = 0;
		m_telemetry.bodyAngularVelocity.roll       = 0;
		m_telemetry.bodyAngularVelocity.yaw        = 0;
	}
	else
	{
		lowPass<float>(m_telemetry.bodyLinearAcceleration.forward, (forwardSpeed - m_prevSurgeSpeed) / deltaTime, FSMI_VT_ACC_LOW_PASS_FACTOR);
		lowPass<float>(m_telemetry.bodyLinearAcceleration.right,   (rightSpeed - m_prevSwaySpeed) / deltaTime, FSMI_VT_ACC_LOW_PASS_FACTOR);
		lowPass<float>(m_telemetry.bodyLinearAcceleration.upward,  (upSpeed - m_prevHeaveSpeed) / deltaTime, FSMI_VT_ACC_LOW_PASS_FACTOR);

		auto rotationEulerDeg = currentTransform.GetRelativeTransform(m_prevTransform).GetRotation().Euler();

		lowPass<float>(m_telemetry.bodyAngularVelocity.pitch, FMath::DegreesToRadians(rotationEulerDeg.Y / deltaTime), FSMI_VT_ANGLES_SPEED_LOW_PASS_FACTOR);
		lowPass<float>(m_telemetry.bodyAngularVelocity.roll,  FMath::DegreesToRadians(rotationEulerDeg.X / deltaTime), FSMI_VT_ANGLES_SPEED_LOW_PASS_FACTOR);
		lowPass<float>(m_telemetry.bodyAngularVelocity.yaw,   FMath::DegreesToRadians(rotationEulerDeg.Z / deltaTime), FSMI_VT_ANGLES_SPEED_LOW_PASS_FACTOR);
	}

	m_prevSurgeSpeed = forwardSpeed;
	m_prevSwaySpeed  = rightSpeed;
	m_prevHeaveSpeed = upSpeed;
	m_prevTransform  = currentTransform;

	if (m_api)
	{
		m_api->SendTelemetryACE(&m_telemetry);
	}
}

void ForceSeatMIPhysXVehicle_ControllerCore::Pause(bool paused)
{
	m_firstCall = true;

	if (paused && m_api)
	{
		FSMI_TelemetryACE pauseTelemetry;
		memset(&pauseTelemetry, 0, sizeof(pauseTelemetry));
		pauseTelemetry.structSize = sizeof(pauseTelemetry);
		pauseTelemetry.state      = paused ? FSMI_STATE_PAUSE : FSMI_STATE_NO_PAUSE;

		m_api->SendTelemetryACE(&pauseTelemetry);
	}
}

void ForceSeatMIPhysXVehicle_ControllerCore::AddExtraTransformation(const ExtraTransformation& transformation)
{
	m_telemetry.extraRotation.yaw        = transformation.yaw;
	m_telemetry.extraRotation.pitch      = transformation.pitch;
	m_telemetry.extraRotation.roll       = transformation.roll;
	m_telemetry.extraTranslation.right   = transformation.right;
	m_telemetry.extraTranslation.upward  = transformation.up;
	m_telemetry.extraTranslation.forward = transformation.forward;
}

void ForceSeatMIPhysXVehicle_ControllerCore::Begin()
{
	m_firstCall = true;

	if (m_api)
	{
		m_api->BeginMotionControl();
	}
}

void ForceSeatMIPhysXVehicle_ControllerCore::End()
{
	m_firstCall = true;

	if (m_api)
	{
		Pause(false);

		m_api->EndMotionControl();
	}
}

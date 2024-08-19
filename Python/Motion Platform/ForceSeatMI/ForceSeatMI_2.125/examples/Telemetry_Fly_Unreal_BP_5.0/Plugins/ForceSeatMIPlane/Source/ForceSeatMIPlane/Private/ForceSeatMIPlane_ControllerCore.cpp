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

#include "ForceSeatMIPlane_ControllerCore.h"
#include "IForceSeatMI.h"
#include "IForceSeatMI_API.h"

#include "GameFramework/Pawn.h"
#include "Engine/World.h"

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

	constexpr const float FSMI_PT_ACC_LOW_PASS_FACTOR          = 0.5f;
	constexpr const float FSMI_PT_ANGLES_SPEED_LOW_PASS_FACTOR = 0.7f;
}

ForceSeatMIPlane_ControllerCore::ForceSeatMIPlane_ControllerCore(APawn* plane)
	: m_plane(plane)
	, m_world(nullptr)
	, m_api(IForceSeatMI::Get().CreateAPI("SDK - Plane Telemetry ACE"))
	, m_firstCall(true)
{
	memset(&m_telemetry, 0, sizeof(m_telemetry));
	m_telemetry.structSize = sizeof(m_telemetry);

	if (m_plane->IsValidLowLevel())
	{
		m_world = m_plane->GetWorld();
	}
}

ForceSeatMIPlane_ControllerCore::~ForceSeatMIPlane_ControllerCore()
{
	delete m_api;
	m_api = nullptr;
}

void ForceSeatMIPlane_ControllerCore::Begin()
{
	m_firstCall = true;

	if (m_api)
	{
		m_api->BeginMotionControl();
	}
}

void ForceSeatMIPlane_ControllerCore::End()
{
	m_firstCall = true;

	if (m_api)
	{
		Pause(false);

		m_api->EndMotionControl();
	}
}

void ForceSeatMIPlane_ControllerCore::Update(float deltaTime)
{
	if (!m_plane)
	{
		return;
	}

	if (m_plane->IsValidLowLevel() && m_world != nullptr)
	{
		if (deltaTime < 0.0001f)
		{
			return;
		}

		auto currentLocation = m_plane->GetActorLocation();

		const auto& currentTransform = m_plane->GetActorTransform();

		currentLocation.X += m_world->OriginLocation.X;
		currentLocation.Y += m_world->OriginLocation.Y;
		currentLocation.Z += m_world->OriginLocation.Z;

		if (m_firstCall)
		{
			m_firstCall = false;

			m_telemetry.vehicleForwardSpeed            = 0;
			m_telemetry.bodyLinearAcceleration.forward = 0;
			m_telemetry.bodyLinearAcceleration.right   = 0;
			m_telemetry.bodyLinearAcceleration.upward  = 0;
			m_telemetry.bodyAngularVelocity.pitch      = 0;
			m_telemetry.bodyAngularVelocity.roll       = 0;
			m_telemetry.bodyAngularVelocity.yaw        = 0;

			m_prevForwardSpeed = 0;
			m_prevRightSpeed   = 0;
			m_prevUpSpeed      = 0;
		}
		else
		{
			FVector velocity;
			velocity.X = (currentLocation.X - m_prevLocation.X) / deltaTime;
			velocity.Y = (currentLocation.Y - m_prevLocation.Y) / deltaTime;
			velocity.Z = (currentLocation.Z - m_prevLocation.Z) / deltaTime;

			auto forwardSpeed = toMeters(FVector::DotProduct(m_plane->GetActorForwardVector(), velocity));
			auto rightSpeed   = toMeters(FVector::DotProduct(m_plane->GetActorRightVector(), velocity));
			auto upSpeed      = toMeters(FVector::DotProduct(m_plane->GetActorUpVector(), velocity));

			/*
			 * This log shows that the velocity values stay the same even when the origin is rebased and the position
			 * of the aeroplane pawn "jumps" because it's position is based on a new origin after rebasing
			 */
			 //UE_LOG(LogTemp, Warning, TEXT("Origin Location X=%d, Y=%d, airplane velocity X=%f, Y=%f"), m_world->OriginLocation.X, m_world->OriginLocation.Y, velocity.X, velocity.Y)

			m_telemetry.vehicleForwardSpeed = forwardSpeed;

			lowPass<float>(m_telemetry.bodyLinearAcceleration.forward, (forwardSpeed - m_prevForwardSpeed) / deltaTime, FSMI_PT_ACC_LOW_PASS_FACTOR);
			lowPass<float>(m_telemetry.bodyLinearAcceleration.right,   (rightSpeed - m_prevRightSpeed) / deltaTime, FSMI_PT_ACC_LOW_PASS_FACTOR);
			lowPass<float>(m_telemetry.bodyLinearAcceleration.upward,  (upSpeed - m_prevUpSpeed) / deltaTime, FSMI_PT_ACC_LOW_PASS_FACTOR);

			auto rotationEulerDeg = currentTransform.GetRelativeTransform(m_prevTransform).GetRotation().Euler();

			lowPass<float>(m_telemetry.bodyAngularVelocity.pitch, FMath::DegreesToRadians(rotationEulerDeg.Y / deltaTime), FSMI_PT_ANGLES_SPEED_LOW_PASS_FACTOR);
			lowPass<float>(m_telemetry.bodyAngularVelocity.roll, FMath::DegreesToRadians(rotationEulerDeg.X / deltaTime), FSMI_PT_ANGLES_SPEED_LOW_PASS_FACTOR);
			lowPass<float>(m_telemetry.bodyAngularVelocity.yaw, FMath::DegreesToRadians(rotationEulerDeg.Z / deltaTime), FSMI_PT_ANGLES_SPEED_LOW_PASS_FACTOR);

			m_prevForwardSpeed = forwardSpeed;
			m_prevRightSpeed   = rightSpeed;
			m_prevUpSpeed      = upSpeed;
		}

		m_prevTransform = currentTransform;
		m_prevLocation = currentLocation;
	}

	if (m_api)
	{
		m_api->SendTelemetryACE(&m_telemetry);
	}
}

void ForceSeatMIPlane_ControllerCore::Pause(bool paused)
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

void ForceSeatMIPlane_ControllerCore::AddExtraTransformation(const ExtraTransformation& transformation)
{
	m_telemetry.extraRotation.yaw        = transformation.yaw;
	m_telemetry.extraRotation.pitch      = transformation.pitch;
	m_telemetry.extraRotation.roll       = transformation.roll;
	m_telemetry.extraTranslation.right   = transformation.right;
	m_telemetry.extraTranslation.upward  = transformation.up;
	m_telemetry.extraTranslation.forward = transformation.forward;
}

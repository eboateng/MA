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

// Disable PhysX deprecation related warnings (check ChaosVehiclePhysics Plugin)
#pragma warning(disable : 4996)

#include "WheeledVehicle.h"

#include "IForceSeatMI_API.h"
#include "IForceSeatMIPhysXVehicle_ControllerCore.h"
#include "ForceSeatMI_TelemetryACE.h"

class ForceSeatMIPhysXVehicle_ControllerCore : public IForceSeatMIPhysXVehicle_ControllerCore
{
	explicit ForceSeatMIPhysXVehicle_ControllerCore(const ForceSeatMIPhysXVehicle_ControllerCore& other) = delete;
	explicit ForceSeatMIPhysXVehicle_ControllerCore(const ForceSeatMIPhysXVehicle_ControllerCore&& other) = delete;

	ForceSeatMIPhysXVehicle_ControllerCore& operator=(const ForceSeatMIPhysXVehicle_ControllerCore& other) = delete;
	ForceSeatMIPhysXVehicle_ControllerCore& operator=(const ForceSeatMIPhysXVehicle_ControllerCore&& other) = delete;

public:
	explicit ForceSeatMIPhysXVehicle_ControllerCore(AWheeledVehicle* vehicle);
	virtual ~ForceSeatMIPhysXVehicle_ControllerCore();

public:
	virtual void Begin() override;
	virtual void End() override;
	virtual void Update(float deltaTime) override;
	virtual void Pause(bool paused) override;
	virtual void AddExtraTransformation(const ExtraTransformation& transformation) override;

private:
	AWheeledVehicle*  m_vehicle;
	IForceSeatMI_API* m_api;
	bool              m_firstCall;
	FSMI_TelemetryACE m_telemetry;
	FTransform        m_prevTransform;
	float             m_prevSurgeSpeed;
	float             m_prevSwaySpeed;
	float             m_prevHeaveSpeed;
};

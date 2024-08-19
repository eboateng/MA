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

#include "IForceSeatMI_TelemetryObject.h"
#include "TransformVectorized.h"
#include "CarSimMovementComponent.h"

class ACarsimPawn;

class ForceSeatMI_CarSim : public IForceSeatMI_TelemetryObject
{
	explicit ForceSeatMI_CarSim(const ForceSeatMI_CarSim& other) = delete;
	explicit ForceSeatMI_CarSim(const ForceSeatMI_CarSim&& other) = delete;

public:
	ForceSeatMI_CarSim(const ACarsimPawn& car, float accLPF, float angleSpeedLPF);

	void setAccelerationLowPassFactor(float factor);
	void setAngleSpeedLowPassFactor(float factor);

public:
	virtual void Begin();
	virtual void End();
	virtual void Update(float deltaTime);
	virtual void Pause(bool paused);
	virtual const FSMI_TelemetryACE& GetTelemetry() const;

private:
	void InitializeHandles();
	void DebugValue(const wchar_t* name, float value);

private:
	const ACarsimPawn& m_car;
	FSMI_TelemetryACE  m_telemetry;
	bool               m_firstCall;

	FVsVarHandle       m_rpmHandle;
	FVsVarHandle       m_gearNumberHandle;

	FVsVarHandle       m_surgeSpeedHandle;
	FVsVarHandle       m_swaySpeedHandle;
	FVsVarHandle       m_heaveSpeedHandle;
	FVsVarHandle       m_surgeAccHandle;
	FVsVarHandle       m_swayAccHandle;
	FVsVarHandle       m_heaveAccHandle;

	FVsVarHandle       m_pitchHandle;
	FVsVarHandle       m_rollHandle;
	FVsVarHandle       m_yawHandle;
	FVsVarHandle       m_pitchSpeedHandle;
	FVsVarHandle       m_rollSpeedHandle;
	FVsVarHandle       m_yawSpeedHandle;

	float              m_accLPF;
	float              m_angleSpeedLPF;
};


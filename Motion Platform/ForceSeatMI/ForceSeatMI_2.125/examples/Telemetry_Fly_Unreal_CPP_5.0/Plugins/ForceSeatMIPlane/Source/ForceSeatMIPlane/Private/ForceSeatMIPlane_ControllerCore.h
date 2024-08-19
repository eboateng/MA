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

#include "IForceSeatMIPlane_ControllerCore.h"
#include "ForceSeatMI_TelemetryACE.h"

class APawn;
class UWorld;
class IForceSeatMI_API;

class ForceSeatMIPlane_ControllerCore : public IForceSeatMIPlane_ControllerCore
{
	explicit ForceSeatMIPlane_ControllerCore(const ForceSeatMIPlane_ControllerCore& other) = delete;
	explicit ForceSeatMIPlane_ControllerCore(const ForceSeatMIPlane_ControllerCore&& other) = delete;

	ForceSeatMIPlane_ControllerCore& operator=(const ForceSeatMIPlane_ControllerCore& other) = delete;
	ForceSeatMIPlane_ControllerCore& operator=(const ForceSeatMIPlane_ControllerCore&& other) = delete;

public:
	explicit ForceSeatMIPlane_ControllerCore(APawn* plane);
	virtual ~ForceSeatMIPlane_ControllerCore();

public:
	virtual void Begin() override;
	virtual void End() override;
	virtual void Update(float deltaTime) override;
	virtual void Pause(bool paused) override;
	virtual void AddExtraTransformation(const ExtraTransformation& transformation) override;

private:
	APawn*            m_plane;
	UWorld*           m_world;
	IForceSeatMI_API* m_api;
	bool              m_firstCall;
	FSMI_TelemetryACE m_telemetry;
	FTransform        m_prevTransform;
	FVector           m_prevLocation;
	float             m_prevForwardSpeed;
	float             m_prevRightSpeed;
	float             m_prevUpSpeed;
};

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

#include "MotionHost.h"

#include "ForceSeatMI_TelemetryACE.h"

class IForceSeatMI_API;

class ForceSeatMI_MotionHost : public MotionHost
{
public:
	explicit ForceSeatMI_MotionHost(const char* profile = nullptr, const char* appID = nullptr);
	virtual ~ForceSeatMI_MotionHost();

public:
	virtual void Initialize() override;
	virtual void Cleanup() override;
	virtual void ConnectToController(FString ControllerIP, int ControllerPort, int HostRcvPort) override;
	virtual void ReceiveDataFromController() override;
	virtual void SetStateFlow() override;
	virtual void SendDataToController(FMotionData& Data) override;

private:
	virtual void MapData(FMotionData& Data) override;

private:
	IForceSeatMI_API* m_api;
	FSMI_TelemetryACE m_telemetry;
};

/*
 * Copyright (C) 2012-2022 MotionSystems
 * 
 * This file is part of ForceSeatMI SDK.
 *
 * www.motionsystems.eu
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include "ForceSeatMI_Functions.h"

static void append(std::wstring& str, const wchar_t* text)
{
	if (! str.empty())
	{
		str += L" ";
	}
	str += text;
}

static const wchar_t* statusToString(FSMI_UINT8 status)
{
	switch (status)
	{
		case FSMI_ModuleStatus_Ok:                   return L"Ok";
		case FSMI_ModuleStatus_Overheated:           return L"Overheated";
		case FSMI_ModuleStatus_Communication_Error:  return L"Communication Error";
		case FSMI_ModuleStatus_Config_Error:         return L"Config Error";
		case FSMI_ModuleStatus_RefPointSwitch_Error: return L"Limit Switch Error";
		case FSMI_ModuleStatus_Calibration_Error:    return L"Calibration Error";
		case FSMI_ModuleStatus_General_Error:        return L"General Error";
		case FSMI_ModuleStatus_NotConnected_Error:   return L"Not connected";
		case FSMI_ModuleStatus_NoPowerSupply_Error:  return L"No power supply error";
		case FSMI_ModuleStatus_FanSpeedTooLow_Error: return L"Fan speed too low error";
		case FSMI_ModuleStatus_OutOfRange_Error:     return L"Out of range error";
		case FSMI_ModuleStatus_StallDetected_Error:  return L"Stall detected error";
		case FSMI_ModuleStatus_EMCY_Error:           return L"EMCY error";
		default:                                     return L"Unknown";
	}
}

static std::wstring stateToString(FSMI_UINT8 state)
{
	std::wstring result;
	if ((state & FSMI_PlatformCurrentState_AnyPaused) != 0)
	{
		append(result, L"paused");
	}
	if ((state & FSMI_PlatformCurrentState_Offline) != 0)
	{
		append(result, L"offline");
	}
	auto parkMode = state & FSMI_PlatformCurrentState_ParkModeMask;
	switch (parkMode)
	{
		case FSMI_PlatformCurrentState_SoftParkToCenter:
			append(result, L"soft center");
			break;
		case FSMI_PlatformCurrentState_SoftParkNormal:
			append(result, L"soft normal");
			break;
		case FSMI_PlatformCurrentState_SoftParkForTransport:
			append(result, L"soft transport");
			break;
		case FSMI_PlatformCurrentState_OperatorParkToCenter:
			append(result, L"operator center");
			break;
		case FSMI_PlatformCurrentState_OperatorParkNormal:
			append(result, L"operator normal");
			break;
		case FSMI_PlatformCurrentState_OperatorParkForTransport:
			append(result, L"operator transport");
			break;
		default:
			break;
	}

	if (parkMode != FSMI_PlatformCurrentState_NoParking && (state & FSMI_PlatformCurrentState_ParkingCompleted) != 0)
	{
		append(result, L"(completed)");
	}

	return result.empty() ? L"running" : result;
}

static void work(FSMI_Handle api)
{

	printf("Giving time for the client to connect...\n");
	Sleep(3000);

	// Communication structure
	FSMI_TopTablePositionPhysical pos;
	FSMI_PlatformInfo platformInfo;

	// Demo data
	float x = 0;
	float step = 0.01f;

	// Prepare structure by clearing it and setting correct size
	memset(&pos, 0, sizeof(pos));
	pos.structSize = sizeof(pos);

	// The demo program is able to provide pause, position and speed limit
	pos.mask = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;

	// Let's start the motion
	ForceSeatMI_BeginMotionControl(api);

	printf("SIM started...\n");
	printf("Press 'q' to exit\n");

	FSMI_UINT64 recentMark = 0;

	while (GetKeyState('Q') == 0)
	{
		pos.state = FSMI_STATE_NO_PAUSE;
		pos.maxSpeed = 45000;

		// Fill demo data
		pos.pitch = 0; // in rad
		pos.roll  = x; // in rad
		pos.yaw   = 0; // in rad
		pos.sway  = 0; // in mm
		pos.surge = 0; // in mm
		pos.heave = 0; // in mm

		ForceSeatMI_SendTopTablePosPhy(api, &pos);

		// Change values somehow
		if (step > 0)
		{
			if (x < 0.3f)
			{
				x += step;
			}
			else
			{
				step = -step;
			}
		}
		else
		{
			if (x > -0.3f)
			{
				x += step;
			}
			else
			{
				step = -step;
			}
		}

		// Get current status
		if (ForceSeatMI_GetPlatformInfoEx(api, &platformInfo, sizeof(platformInfo), 100))
		{
			if (platformInfo.structSize != sizeof(platformInfo))
			{
				wprintf(L"Incorrect structure size: %d vs %d\n", (int)platformInfo.structSize, (int)sizeof(platformInfo));
				break;
			}
			else if (platformInfo.timemark == recentMark)
			{
				wprintf(L"No new platform info\n");
			}
			else
			{
				wprintf(L"Connected: %ls, State: %ls, Pos: %u, %u, %u, %u, %u, %u, Time: %llu, Module %u status: %ls\n",
					platformInfo.isConnected != FSMI_False ? L"yes" : L"no",
					stateToString(platformInfo.state).c_str(),
					platformInfo.actualMotorPosition[0],
					platformInfo.actualMotorPosition[1],
					platformInfo.actualMotorPosition[2],
					platformInfo.actualMotorPosition[3],
					platformInfo.actualMotorPosition[4],
					platformInfo.actualMotorPosition[5],
					platformInfo.timemark,
					platformInfo.worstModuleStatusIndex,
					statusToString(platformInfo.worstModuleStatus));

				wprintf(L"    Actual Top Frame: %ls roll %f, pitch %f, yaw %f, heave %f, surge %f, sway %f\n",
					platformInfo.fkRecentStatus ? L"ok" : L"failed",
					platformInfo.fkRoll,
					platformInfo.fkPitch,
					platformInfo.fkYaw,
					platformInfo.fkHeave,
					platformInfo.fkSurge,
					platformInfo.fkSway);

				recentMark = platformInfo.timemark;
			}
		}
		else
		{
			wprintf(L"Failed to get platform info\n");
		}

		Sleep(5);
	}
	ForceSeatMI_EndMotionControl(api);
	printf("Game ended...\n");
}

int main(int argc, wchar_t* argv[])
{
	FSMI_Handle api = ForceSeatMI_Create();
	if (! api)
	{
		wprintf(L"ForceSeatMI DLL was not loaded, so the game will not send anything to the ForceSeatPM\n");
		return -1;
	}

	// Activate appropriate ForceSeatPM profile
	ForceSeatMI_SetAppID(api, ""); // If you have dedicated app id, remove ActivateProfile calls from your code
	ForceSeatMI_ActivateProfile(api, "SDK - Positioning");

	work(api);
	ForceSeatMI_Delete(api);
	return 0;
}

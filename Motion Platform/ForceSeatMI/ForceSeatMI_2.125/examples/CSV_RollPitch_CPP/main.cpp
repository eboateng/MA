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

#include <Windows.h>
#include <stdio.h>

#include "ForceSeatMI_Functions.h"
#include "MSU_GyroPlayer.h"

struct CallbackContext
{
	FSMI_TopTablePositionPhysical* pos;
	FSMI_Handle api;
};

static const unsigned int INTERVAL = 10 /* miliseconds*/;

static void callback(const MSU_GyroPlayerData* data, void* userData)
{
	auto context = reinterpret_cast<CallbackContext*>(userData);

	context->pos->state = FSMI_STATE_NO_PAUSE;
	context->pos->roll  = data->roll  * 3.1415f/180.0f;
	context->pos->pitch = data->pitch * 3.1415f/180.0f;

	ForceSeatMI_SendTopTablePosPhy(context->api, context->pos);
}

static void work(FSMI_Handle api, MSU_GP_Handle player)
{
	printf("Giving time for the client to connect...\n");
	Sleep(3000);

	// Skip header line in CSV
	MSU_GyroPlayer_SetCsvSkipLines(player, 1);

	// Define columns meaning in CSV file
	MSU_GyroPlayer_SetCsvMapping(player, 1, MSU_GPR_Yaw,   MSU_GPR_NotInverted/* no invertion*/, 1.0f);
	MSU_GyroPlayer_SetCsvMapping(player, 2, MSU_GPR_Pitch, MSU_GPR_NotInverted/* no invertion*/, 1.0f);
	MSU_GyroPlayer_SetCsvMapping(player, 3, MSU_GPR_Roll,  MSU_GPR_NotInverted/* no invertion*/, 1.0f);

	// Load CSV data
	auto result = MSU_GyroPlayer_LoadCsv(player, L"sample_rotation.csv", ';', INTERVAL, MSU_GPR_Seconds);

	if (result != MSU_GPEC_Ok)
	{
		printf("Failed to load CSV, error code: %d\n", result);
		return;
	}

	// Communication structure
	FSMI_TopTablePositionPhysical pos;
	memset(&pos, 0, sizeof(pos));
	pos.structSize = sizeof(pos);

	// The demo program provides roll and pitch
	pos.mask = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;
	pos.maxSpeed = 65535;

	// Let's start the motion
	ForceSeatMI_BeginMotionControl(api);

	printf("Playing started...\n");

	CallbackContext context;
	context.api = api;
	context.pos = &pos;
	
	result = MSU_GyroPlayer_PlayAsync(player, &callback, &context);

	if (result != MSU_GPEC_Ok)
	{
		printf("Failed to play CSV, error code: %d\n", result);
		return;
	}

	MSU_GyroPlayer_Wait(player);
	ForceSeatMI_EndMotionControl(api);
}

int main(int argc, wchar_t* argv[])
{
	auto api = ForceSeatMI_Create();
	if (! api)
	{
		wprintf(L"ForceSeatMI DLL was not loaded, so the game will not send anything to the ForceSeatPM\n");
		return -1;
	}

	auto player = MSU_GyroPlayer_Create();
	if (!player)
	{
		wprintf(L"MSU DLL was not loaded, so the game will not send anything to the ForceSeatPM\n");
		ForceSeatMI_Delete(api);
		return 0;
	}

	// Activate appropriate ForceSeatPM profile
	ForceSeatMI_SetAppID(api, ""); // If you have dedicated app id, remove ActivateProfile calls from your code
	ForceSeatMI_ActivateProfile(api, "SDK - Positioning");

	work(api, player);
	MSU_GyroPlayer_Delete(player);
	ForceSeatMI_Delete(api);
	return 0;
}


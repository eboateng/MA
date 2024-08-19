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

static void work(FSMI_Handle api)
{
	
	printf("Giving time for the client to connect...\n");
	Sleep(3000);

	// Communication structure
	FSMI_TopTablePositionPhysical pos;
	FSMI_SFX sfx;

	// Demo data
	float x = 0;
	float step = 0.01f;
	unsigned int sfxIterator = 0;

	// Prepare structure by clearing it and setting correct size
	memset(&pos, 0, sizeof(pos));
	pos.structSize = sizeof(pos);
	memset(&sfx, 0, sizeof(sfx));
	sfx.structSize = sizeof(sfx);

	// The demo program is able to provide pause, position and speed limit
	pos.mask = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;

	// Let's start the motion
	ForceSeatMI_BeginMotionControl(api);

	printf("SIM started...\n");
	printf("Press 'q' to exit\n");

	// Configure SFX
	// Level 2 is supported by all motion platforms, Level 3 and 4 are supported by "QS" motion platforms.
	sfx.effects[0].type = FSMI_SFX_EffectType_SinusLevel2; 
	sfx.effects[0].area = FSMI_SFX_AreaFlags_FrontLeft;
	sfx.effects[0].amplitude = 0.05f;
	sfx.effects[0].frequency = 0;
	sfx.effectsCount = 1;

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
	
		sfx.effects[0].frequency = sfxIterator / 10;

		ForceSeatMI_SendTopTablePosPhy2(api, &pos, &sfx, nullptr);

		// Change values somehow
		++sfxIterator;
		if (sfxIterator >= 500)
		{
			sfxIterator = 0;
		}
		
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

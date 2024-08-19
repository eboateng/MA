/*
 * Copyright (C) 2012-2020 Motion Systems
 *
 * This file is part of ForceSeat motion system.
 *
 * www.motionsystems.eu
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * -------------------------------------------------------------------------------------------------------
 *
 * This example uses ForceSeatDI to show how to connect to 4 motion units at the same time
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "ForceSeatDI.h"

namespace
{
	bool connectUSB(FSDI_Handle& api, const wchar_t* deviceSn)
	{
		api = ForceSeatDI_Create();
		if (api == nullptr)
		{
			printf("ForceSeatDI DLL was not loaded, so the SIM will not send anything to motion platforms\n");
			return false;
		}

		if (ForceSeatDI_ConnectToUsbDevice(api, nullptr, deviceSn) != FSDI_True)
		{
			wprintf(L"Failed to connect to %ls, error: %d\n",
					(deviceSn ? deviceSn : L"NULL"),
					ForceSeatDI_GetRecentErrorCode(api));
			return false;
		}

		FSDI_Char sn[FSDI_SerialNumberStringLength];
		if (ForceSeatDI_GetSerialNumber(api, sn) != FSDI_True)
		{
			printf("Failed to get platform info\n");
			return false;
		}

		printf("Platform S/N: %s\n", reinterpret_cast<const char*>(sn));

		FSDI_Bool isLicenseValid = FSDI_False;
		if (ForceSeatDI_GetLicenseStatus(api, &isLicenseValid) != FSDI_True)
		{
			printf("Failed to get license status\n");
			return false;
		}

		if (isLicenseValid != FSDI_True)
		{
			printf("License is not valid\n");
			return false;
		}
		return true;
	}

	void work(FSDI_Handle& api)
	{
		if (! connectUSB(api, nullptr))
		{
			return;
		}

		Sleep(500);

		FSDI_TopTablePositionLogical  pos;
		memset(&pos, 0, sizeof(pos));

		FSDI_SFX sfx;
		memset(&sfx, 0, sizeof(pos));

		pos.structSize = sizeof(FSDI_TopTablePositionLogical);
		sfx.structSize = sizeof(FSDI_SFX);

		pos.mask = FSDI_BIT_PAUSE | FSDI_BIT_POSITION | FSDI_BIT_MAX_SPEED;
		pos.maxSpeed = 65535;
		pos.pause = FSDI_False;

		// Configure SFX
		// Level 2 is supported by "PS" and "QS" motion platforms, Level 3 and 4 are supported by "QS" motion platforms.
		sfx.effects[0].type = FSDI_SFX_EffectType_SinusLevel2; 
		sfx.effects[0].area = FSDI_SFX_AreaFlags_FrontLeft;
		sfx.effects[0].amplitude = 0.05f;
		sfx.effects[0].frequency = 0;
		sfx.effectsCount = 1;

		unsigned int iterator = 0;

		printf("SIM started...\n");
		printf("Press 'q' to exit\n");
		while (GetKeyState('Q') == 0)
		{
			// Prepare demo data
			auto value = static_cast<FSDI_INT16>(sinf(iterator * 3.1415f / 180) * 32767);
			sfx.effects[0].frequency = iterator / 10;
			if (++iterator > 360)
			{
				iterator = 0;
			}

			pos.roll  = value;
			pos.pitch = 0;

			if (ForceSeatDI_SendTopTablePosLog2(api, &pos, &sfx) != FSDI_True)
			{
				printf("Failed to send request to platform\n");
			}

			Sleep(10);
		}
		printf("SIM ended...\n");
	}
}

int main(int argc, wchar_t* argv[])
{
	(void)argc;
	(void)argv;
	FSDI_Handle api = 0;
	work(api);

	ForceSeatDI_Park(api, FSDI_ParkMode_Normal);
	ForceSeatDI_Delete(api);

	return 0;
}

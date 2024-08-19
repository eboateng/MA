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
#include <string>

#include "UDPServer.h"
#include "ExampleFrame.h"
#include "ForceSeatMI_Functions.h"

/*
 * This example demonstrates how to create a simple UDP proxy that receives incoming UDP frames from local network and
 * convert them into ForceSeatMI calls. Modify both ExampleFrame and DatagramArrived for your needs.
 * 
 * ATTENTION! This is not a fully working solution. It is a TEMPLATE that allows to create your own UDP proxy application.
 */
namespace
{
	// This example uses positioning mode but the same approach can be used for telemetry mode
	FSMI_TopTablePositionPhysical g_pos; 

	// Global API object so it is easier to access
	FSMI_Handle g_api = nullptr;
}

static void DatagramArrived(const char* buffer, unsigned int size);

int main(int argc, wchar_t* argv[])
{
	(void)argc;
	(void)argv;

	g_api = ForceSeatMI_Create();
	if (! g_api)
	{
		printf("ForceSeatMI DLL was not loaded, so the game will not send anything to the ForceSeatPM\n");
		return -1;
	}

	// Activate appropriate ForceSeatPM profile
	ForceSeatMI_ActivateProfile(g_api, "SDK - Positioning");

	printf("Giving time for ForceSeatPM module to start...\n");
	Sleep(3000);

	// Prepare structure by clearing it and setting correct size
	memset(&g_pos, 0, sizeof(g_pos));
	g_pos.structSize = sizeof(g_pos);
	g_pos.mask       = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;
	g_pos.state      = FSMI_STATE_NO_PAUSE;
	g_pos.maxSpeed   = 65535;

	// Let's start the motion control
	ForceSeatMI_BeginMotionControl(g_api);

	// Start UDP server
	UDPServer().exec(20777 /* port */, &DatagramArrived);

	// Cleanup
	ForceSeatMI_EndMotionControl(g_api);
	ForceSeatMI_Delete(g_api);
	return 0;
}

static void DatagramArrived(const char* buffer, unsigned int size)
{
	if (g_api != nullptr && buffer != nullptr)
	{
		if (size != sizeof(ExampleFrame))
		{
			printf("Incorrect message size %u vs %u\n", size, sizeof(ExampleFrame));
			return;
		}

		const auto* frame = reinterpret_cast<const ExampleFrame*>(buffer);
		if (frame->structSize != sizeof(ExampleFrame))
		{
			printf("Incorrect structSize %u vs %u\n", frame->structSize, sizeof(ExampleFrame));
			return;
		}

		if (frame->magicHeader != MAGIC_HEADER || frame->magicFooter != MAGIC_FOOTER)
		{
			printf("Incorrect header or footer\n");
			return;
		}

		// Copy from UDP frame to FSMI structure and call FSMI function
		g_pos.roll  = frame->roll;
		g_pos.pitch = frame->pitch;
		g_pos.yaw   = frame->yaw;
		g_pos.heave = frame->heave;
		g_pos.sway  = frame->sway;
		g_pos.surge = frame->surge;

		ForceSeatMI_SendTopTablePosPhy(g_api, &g_pos);
	}
}

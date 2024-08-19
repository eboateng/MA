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

#include <chrono>
#include <thread>
#include <stdio.h>

#include "ForceSeatMI_Functions.h"
#include "MSU_GyroPlayer.h"

namespace
{
	static constexpr const int MAX_SPEED           = 65535;
	static constexpr const int SAMPLES_INTERVAL_MS = 0; // zero means that no sample are ignored and GyroPlayer does not wait between callback calls
	static constexpr const int MSG_INTERVAL_MS     = 4;

	class Timer
	{
	public:
		typedef std::chrono::high_resolution_clock::time_point TimePoint;

		Timer()
		{
			Restart();
		}

		void Restart()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		uint64_t Elapsed_ms() const
		{
			auto now = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed(now - m_start);
			return static_cast<uint64_t>(elapsed.count());
		}

		void WaitUntil_ms(uint64_t time)
		{
			auto awakeTime = m_start + std::chrono::milliseconds(time);
			std::this_thread::sleep_until(awakeTime);
		}

	private:
		TimePoint m_start;
	};

	struct CallbackContext
	{
		FSMI_Handle                    api;
		FSMI_TopTablePositionPhysical* position;
		Timer                          timer;
		uint64_t                       lastMsgTimeMark_us;
		uint64_t                       previousSampleTimeMark_us;
		bool                           firstCall;
		
		struct
		{
			float velocity;
			float displacement;
		} sway, surge, heave;
	};

	void callback(const MSU_GyroPlayerData* data, void* userData)
	{
		auto* context = reinterpret_cast<CallbackContext*>(userData);

		if (context->firstCall)
		{ 
			context->firstCall                 = false;
			context->previousSampleTimeMark_us = data->timestamp_us;
			context->lastMsgTimeMark_us        = data->timestamp_us;;
			context->timer.Restart();
		}

		// Interval between samples might not be constant, so calculate dT each callback call
		float dTime_s = 0.000001f * (data->timestamp_us - context->previousSampleTimeMark_us);
		context->previousSampleTimeMark_us = data->timestamp_us;

		context->sway.displacement   += context->sway.velocity  * dTime_s + data->accSway  * (dTime_s * dTime_s * 0.5f);
		context->surge.displacement  += context->surge.velocity * dTime_s + data->accSurge * (dTime_s * dTime_s * 0.5f);
		context->heave.displacement  += context->heave.velocity * dTime_s + data->accHeave * (dTime_s * dTime_s * 0.5f);

		context->sway.velocity  += data->accSway  * dTime_s;
		context->surge.velocity += data->accSurge * dTime_s;
		context->heave.velocity += data->accHeave * dTime_s;

		context->position->roll  = 0;
		context->position->pitch = 0;
		context->position->yaw   = 0;
		context->position->sway  = context->sway.displacement  * 1000 /* m to mm */;
		context->position->surge = context->surge.displacement * 1000 /* m to mm */;
		context->position->heave = context->heave.displacement * 1000 /* m to mm */;

		// NOTE: Without a washout algorithm, the top table may leave the work area quite quickly 
		// if input data recording was not started when the vehicle was at rest. In other words, 
		// the vehicle's velocities and accelerations should be 0 when input recording begins, 
		// otherwise the pre-existing vehicle velocities and accelerations will be unknown, 
		// and as a result, the top table may drift away from the center.

		if (data->timestamp_us - context->lastMsgTimeMark_us >= 1000 * MSG_INTERVAL_MS)
		{
			// If SAMPLES_INTERVAL_MS is set to 0, then MSU_GyroPlayer does not waits between 'callback' calls.
			// It means that in 'callback' function we need to make sure that new control data is not sent too often.
			// In other words, we want to do math on all samples to achieve higer precision but only send data
			// to the motion platform every MSG_INTERVAL_MS.
			context->timer.WaitUntil_ms(data->timestamp_us / 1000);
			context->lastMsgTimeMark_us = data->timestamp_us;

			ForceSeatMI_SendTopTablePosPhy(context->api, context->position);
		}
	}
}

static void work(FSMI_Handle api, MSU_GP_Handle player)
{
	printf("Giving time for the client to connect...\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	// Skip header line in CSV
	MSU_GyroPlayer_SetCsvSkipLines(player, 1);

	// Define columns meaning in CSV file
	MSU_GyroPlayer_SetCsvMapping(player, 1, MSU_GPR_AccSway,  MSU_GPR_NotInverted /* no invertion*/, 1.0f);
	MSU_GyroPlayer_SetCsvMapping(player, 2, MSU_GPR_AccSurge, MSU_GPR_NotInverted /* no invertion*/, 1.0f);
	MSU_GyroPlayer_SetCsvMapping(player, 3, MSU_GPR_AccHeave, MSU_GPR_NotInverted /* no invertion*/, 1.0f);

	// Load CSV data
	auto result = MSU_GyroPlayer_LoadCsv(player, L"sample_gyro.csv", ';', SAMPLES_INTERVAL_MS, MSU_GPR_Seconds);

	if (result != MSU_GPEC_Ok)
	{
		printf("Failed to load CSV, error code: %d\n", result);
		return;
	}

	// Communication structure
	FSMI_TopTablePositionPhysical position;
	memset(&position, 0, sizeof(position));
	position.structSize = sizeof(position);
	position.mask       = FSMI_POS_BIT_STATE | FSMI_POS_BIT_POSITION | FSMI_POS_BIT_MAX_SPEED;
	position.state      = FSMI_STATE_NO_PAUSE;
	position.maxSpeed   = MAX_SPEED;

	// Let's start the motion
	ForceSeatMI_BeginMotionControl(api);

	printf("Playing started...\n");

	CallbackContext context;
	context.api                       = api;
	context.position                  = &position;
	context.lastMsgTimeMark_us        = 0;
	context.previousSampleTimeMark_us = 0;
	context.firstCall                 = true;
	memset(&context.sway,  0, sizeof(context.sway));
	memset(&context.surge, 0, sizeof(context.surge));
	memset(&context.heave, 0, sizeof(context.heave));
	
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


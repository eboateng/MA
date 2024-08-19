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
using MotionSystems;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Telemetry_CS
{
	class Program
	{
		[STAThread]
		static void Main(string[] args)
		{
			using (ForceSeatMI mi = new ForceSeatMI())
			{
				if (!mi.IsLoaded())
				{
					Console.WriteLine("ForceSeatMI library has not been found! Please install ForceSeatPM.");
					return;
				}

				// Activate appropriate ForceSeatPM profile
				mi.SetAppID(""); // If you have dedicated app id, remove ActivateProfile calls from your code
				mi.ActivateProfile("SDK - Vehicle Telemetry ACE");

				Work(mi);
			}
		}

		static string ToString(FSMI_ModuleStatus status)
		{ 
			switch (status)
			{
				case FSMI_ModuleStatus.Ok:                   return "Ok";
				case FSMI_ModuleStatus.Overheated:           return "Overheated";
				case FSMI_ModuleStatus.Communication_Error:  return "Communication Error";
				case FSMI_ModuleStatus.Config_Error:         return "Config Error";
				case FSMI_ModuleStatus.RefPointSwitch_Error: return "Limit Switch Error";
				case FSMI_ModuleStatus.Calibration_Error:    return "Calibration Error";
				case FSMI_ModuleStatus.General_Error:        return "General Error";
				case FSMI_ModuleStatus.NotConnected_Error:   return "Not connected";
				case FSMI_ModuleStatus.NoPowerSupply_Error:  return "No power supply error";
				case FSMI_ModuleStatus.FanSpeedTooLow_Error: return "Fan speed too low error";
				case FSMI_ModuleStatus.OutOfRange_Error:     return "Out of range error";
				case FSMI_ModuleStatus.StallDetected_Error:  return "Stall detected error";
				case FSMI_ModuleStatus.EMCY_Error:           return "EMCY error";
				default: return "Unknown";
			}
		}

		static void Work(ForceSeatMI mi)
		{
			var telemetry    = FSMI_TelemetryACE.Prepare();
			var sfx          = FSMI_SFX.Prepare();
			var audioEffects = FSMI_TactileAudioBasedFeedbackEffects.Prepare();

			var platformInfo = new FSMI_PlatformInfo();

			Console.WriteLine("Giving time for the client to connect...");
			Thread.Sleep(3000);

			// Demo data
			int iterator = 0;
			int sfxIterator = 0;

			// Configure SFX
			// Level 2 is supported by "PS" and "QS" motion platforms, Level 3 and 4 are supported by "QS" motion platforms.
			sfx.effects[0].type      = (byte)FSMI_SFX_EffectType.SinusLevel2;
			sfx.effects[0].area      = FSMI_SFX_AreaFlags.FrontLeft;
			sfx.effects[0].amplitude = 0.05f;
			sfx.effects[0].frequency = 0;
			sfx.effectsCount         = 1;

			// This demo applicatio does not use any TactileAudioBaseFeedbackEffects

			// Let's start the motion
			mi.BeginMotionControl();

			Console.WriteLine("SIM started...");
			Console.WriteLine("Press 'q' to exit");

			ulong recentMark = 0;

			while (Keyboard.GetKeyStates(System.Windows.Input.Key.Q) == KeyStates.None)
			{
				telemetry.state = FSMI_State.NO_PAUSE;

				telemetry.bodyAngularVelocity[0].yaw   = 0;
				telemetry.bodyAngularVelocity[0].pitch = 0;
				telemetry.bodyAngularVelocity[0].roll  = (float)Math.Sin(iterator / 20.0) * 20000;
				telemetry.bodyLinearAcceleration[0].forward = 0;
				telemetry.bodyLinearAcceleration[0].upward  = 0;
				telemetry.bodyLinearAcceleration[0].right   = (float)Math.Sin(iterator / 3.0)  * 10;

				// Use extra parameters to generate custom effects, for exmp. vibrations. They will NOT be
				// filtered, smoothed or processed in any way.
				telemetry.extraRotation[0].yaw   = 0;
				telemetry.extraRotation[0].pitch = 0;
				telemetry.extraRotation[0].roll  = (float)Math.Sin(iterator) * 0.3f;
				telemetry.extraTranslation[0].right   = 0;
				telemetry.extraTranslation[0].upward  = 0;
				telemetry.extraTranslation[0].forward = 0;

				sfx.effects[0].frequency = (byte)(sfxIterator);
				mi.SendTelemetryACE2(ref telemetry, ref sfx, ref audioEffects);

				// Change values somehow
				++iterator;

				++sfxIterator;
				if (sfxIterator > 50)
				{
					sfxIterator = 0;
				}

				// Get current status
				if (mi.GetPlatformInfoEx(ref platformInfo, (uint)Marshal.SizeOf(platformInfo), 100))
				{
					if (platformInfo.structSize != Marshal.SizeOf(platformInfo))
					{
						Console.WriteLine("Incorrect structure size: {0} vs {1}", platformInfo.structSize, Marshal.SizeOf(platformInfo));
						break;
					}
					else if (platformInfo.timemark == recentMark)
					{
						Console.WriteLine("No new platform info");
					}
					else
					{
						Console.WriteLine("Connected: {0}, Paused: {1}, Pos: {2}, {3}, {4}, {5}, {6}, {7}, Time: {8}, Module {9} status: {10}",
							platformInfo.isConnected != 0 ? "yes" : "no",
							(platformInfo.state & FSMI_PlatformCurrentState.AnyPaused) != 0 ? "yes" : "no",
							platformInfo.actualMotor1_Position,
							platformInfo.actualMotor2_Position,
							platformInfo.actualMotor3_Position,
							platformInfo.actualMotor4_Position,
							platformInfo.actualMotor5_Position,
							platformInfo.actualMotor6_Position,
							platformInfo.timemark,
							platformInfo.worstModuleStatusIndex,
							ToString((FSMI_ModuleStatus)platformInfo.worstModuleStatus));

						Console.WriteLine("    Actual Top Frame: {0} roll {1}, pitch {2}, yaw {3}, heave {4}, surge {5}, sway {6}",
							platformInfo.fkRecentStatus != 0 ? "ok" : "failed",
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
					Console.WriteLine("Failed to get platform info");
				}

				Thread.Sleep(10);
			}
			mi.EndMotionControl();
			Console.WriteLine("Game ended...");
		}

	}
}

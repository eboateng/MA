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

namespace TablePhyPos_CS
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
				mi.ActivateProfile("SDK - Positioning");

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
			var pos          = new FSMI_TopTablePositionPhysical();
			var platformInfo = new FSMI_PlatformInfo();

			Console.WriteLine("Giving time for the client to connect...");
			Thread.Sleep(3000);

			// Demo data
			float x = 0;
			float step = 0.01f;

			// Prepare structure by clearing it and setting correct size
			pos.mask = 0;
			pos.structSize = (byte)Marshal.SizeOf(pos);

			// The demo program is able to provide pause, position and speed limit
			pos.mask = FSMI_POS_BIT.STATE | FSMI_POS_BIT.POSITION | FSMI_POS_BIT.MAX_SPEED;

			// Let's start the motion
			mi.BeginMotionControl();

			Console.WriteLine("SIM started...");
			Console.WriteLine("Press 'q' to exit");

			ulong recentMark = 0;

			while (Keyboard.GetKeyStates(System.Windows.Input.Key.Q) == KeyStates.None)
			{
				pos.state = FSMI_State.NO_PAUSE;
				pos.maxSpeed = 45000;

				// Fill demo data
				pos.pitch = 0; // in rad 
				pos.roll  = x; // in rad 
				pos.yaw   = 0; // in rad
				pos.sway =  0; // in mm 
				pos.surge = 0; // in mm 
				pos.heave = 0; // in mm 

				mi.SendTopTablePosPhy(ref pos);

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

				Thread.Sleep(5);
			}
			mi.EndMotionControl();
			Console.WriteLine("Game ended...");
		}

	}
}

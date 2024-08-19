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
 */

using MotionSystems;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;

namespace TableFastPos_RuntimeLoading_CS
{
	class Program
	{
		[STAThread]
		static void Main(string[] args)
		{
			ForceSeatDI api = new ForceSeatDI(@"..\..\..\.."/* path to DLL, relative to example location */);
			if (! api.IsLoaded())
			{
				api.Dispose();
				Console.WriteLine("ForceSeatDI library has not been found!");
				return;
			}

			Work(ref api);
			api.Park(FSDI_ParkMode.Normal);
			api.Dispose();
		}

		static bool ConnectUSB(ref ForceSeatDI api, string deviceSn)
		{
			if (! api.ConnectToUsbDevice(null, deviceSn))
			{
				Console.WriteLine("Failed to connect to {0}", deviceSn);
				return false;
			}

			string serialNumber = "";
			if (! api.GetSerialNumber(ref serialNumber))
			{
				Console.WriteLine("Failed to get platform info");
				return false;
			}
			Console.WriteLine("Platform S/N: {0}", serialNumber);

			bool isLicenseValid = false;
			if (! api.GetLicenseStatus(ref isLicenseValid))
			{
				Console.WriteLine("Failed to get license status");
				return false;
			}

			if (!isLicenseValid)
			{
				Console.WriteLine("License is not valid");
			}
			return true;
		}

		static string ToString(FSDI_ModuleStatus status)
		{ 
			switch (status)
			{
				case FSDI_ModuleStatus.Ok:                   return "Ok";
				case FSDI_ModuleStatus.Overheated:           return "Overheated";
				case FSDI_ModuleStatus.Communication_Error:  return "Communication Error";
				case FSDI_ModuleStatus.Config_Error:         return "Config Error";
				case FSDI_ModuleStatus.RefPointSwitch_Error: return "Reference Point Switch Error";
				case FSDI_ModuleStatus.Calibration_Error:    return "Calibration Error";
				case FSDI_ModuleStatus.General_Error:        return "General Error";
				case FSDI_ModuleStatus.NotConnected_Error:   return "Not connected";
				case FSDI_ModuleStatus.NoPowerSupply_Error:  return "No power supply error";
				case FSDI_ModuleStatus.FanSpeedTooLow_Error: return "Fan speed too low";
				case FSDI_ModuleStatus.OutOfRange_Error:     return "Out of range";
				default: return "Unknown";
			}
		}

		static void Work(ref ForceSeatDI api)
		{
			if (! ConnectUSB(ref api, null))
			{
				return;
			}

			Thread.Sleep(500);

			FSDI_TopTablePositionLogical pos = new FSDI_TopTablePositionLogical();
			pos.structSize = (byte)Marshal.SizeOf(pos);
			pos.mask = FSDI_BIT.PAUSE | FSDI_BIT.POSITION | FSDI_BIT.MAX_SPEED;
			pos.maxSpeed = 65535;
			pos.pause = 0;

			FSDI_SFX sfx = new FSDI_SFX();
			sfx.structSize = (byte)Marshal.SizeOf(sfx);

			int iterator = 0;

			Console.WriteLine("SIM started...");
			Console.WriteLine("Press 'q' to exit");

			// Configure SFX
			// Level 2 is supported by "PS" and "QS" motion platforms, Level 3 and 4 are supported by "QS" motion platforms.
			sfx.effect1Type = (byte)FSDI_SFX_EffectType.SinusLevel2;
			sfx.effect1Area = FSDI_SFX_AreaFlags.FrontLeft;
			sfx.effect1Amplitude = 0.05f;
			sfx.effect1Frequency = 0;
			sfx.effectsCount = 1;

			while (Keyboard.GetKeyStates(System.Windows.Input.Key.Q) == KeyStates.None)
			{
				// Prepare demo data
				short value = (short)(Math.Sin(iterator * 3.1415f / 180) * 32767);
				sfx.effect1Frequency = (byte)(iterator / 10);
				if (++iterator > 360)
				{
					iterator = 0;
				}

				pos.roll  = value;
				pos.pitch = 0;

				if (! api.SendTopTablePosLog2(ref pos, ref sfx))
				{
					Console.WriteLine("Failed to send request to platform");
				}

				Thread.Sleep(10);
			}
			Console.WriteLine("SIM ended...");
		}
	}
}

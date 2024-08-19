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
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Input;

namespace TablePhyPos_CS_SFX
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

		static void Work(ForceSeatMI mi)
		{
			var pos = new FSMI_TopTablePositionPhysical();
			var sfx = FSMI_SFX.Prepare();
			var audioEffects = FSMI_TactileAudioBasedFeedbackEffects.Prepare();

			Console.WriteLine("Make sure that 'Positioning SDK' profile is active in ForceSeatPM");
			Console.WriteLine("Giving time for the client to connect...");
			Thread.Sleep(3000);

			// Demo data
			int sfxIterator = 0;
			float x = 0;
			float step = 0.01f;

			// Prepare structure by clearing it and setting correct size
			pos.mask = 0;
			pos.structSize = (byte)Marshal.SizeOf(pos);

			// The demo program is able to provide pause, position and speed limit
			pos.mask = FSMI_POS_BIT.STATE | FSMI_POS_BIT.POSITION  | FSMI_POS_BIT.MAX_SPEED;

			// Let's start the motion
			mi.BeginMotionControl();

			Console.WriteLine("SIM started...");
			Console.WriteLine("Press 'q' to exit");

			// Configure SFX
			// Level 2 is supported by all motion platforms, Level 3 and 4 are supported by "QS" motion platforms.
			sfx.effects[0].type = (byte)FSMI_SFX_EffectType.SinusLevel2;
			sfx.effects[0].area = FSMI_SFX_AreaFlags.FrontLeft;
			sfx.effects[0].amplitude = 0.05f;
			sfx.effects[0].frequency = 0;
			sfx.effectsCount = 1;

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

				sfx.effects[0].frequency = (byte)(sfxIterator / 10);

				mi.SendTopTablePosPhy2(ref pos, ref sfx, ref audioEffects);

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

				Thread.Sleep(5);
			}
			mi.EndMotionControl();
			Console.WriteLine("Game ended...");
		}

	}
}

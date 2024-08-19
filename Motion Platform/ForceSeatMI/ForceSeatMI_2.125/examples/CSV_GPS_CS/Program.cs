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
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Input;

namespace CSV_GPS_CS
{
	class Program
	{
		[STAThread]
		static void Main(string[] args)
		{
			var samplingInterval = 0.010 /* 10 miliseconds*/;
			var csv = LoadCSV(@"..\..\..\sample_GPS.csv", samplingInterval);

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

				Work(mi, csv, samplingInterval);
			}
		}

		static T Clamp<T>(T value, T min, T max) where T : IComparable<T>
		{
			if (value.CompareTo(min) < 0)
				return min;
			if (value.CompareTo(max) > 0)
				return max;

			return value;
		}

		static List<List<double>> LoadCSV(string filename, double samplingInterval)
		{
			var csv = new List<List<double>>();

			double starTime = 0;
			double nextTime = 0;
			bool first = true;

			using (var rd = new StreamReader(filename))
			{
				while (!rd.EndOfStream)
				{
					var line = rd.ReadLine().Trim();
					if (line.StartsWith("#") || line.Length == 0)
					{
						// Skip comments and empty lines
						continue;
					}

					var splits = line.Split(',');
					if (splits.Length != 5)
					{
						// Skip incorrect lines
						continue;
					}

					try
					{
						var entry = new List<double>();
						for (int i = 0; i < splits.Length; ++i)
						{
							entry.Add(double.Parse(splits[i], CultureInfo.InvariantCulture));
						}

						// Apply offset for time mark
						entry[0] -= starTime;

						if (first)
						{
							// Remember time from first entry to use it as base
							starTime = entry[0];
							entry[0] = 0;
							csv.Add(entry);

							first = false;

							// Timermark for next sample
							nextTime += samplingInterval;
						}
						else if (entry[0] > nextTime)
						{
							csv.Add(entry);

							// Timermark for next sample
							nextTime += samplingInterval;
						}
					}
					catch (System.FormatException)
					{
						// Line without numbers, skip it
					}
				}
			}
			return csv;
		}

		struct Fields
		{
			public const int Time  = 0;
			public const int Lat   = 1;
			public const int Lon   = 2;
			public const int Alt   = 3;
			public const int Speed = 4;
		}

		static void Work(ForceSeatMI mi, List<List<double>> csv, double samplingInterval)
		{
			FSMI_TopTablePositionLogical pos = new FSMI_TopTablePositionLogical();

			pos.structSize = (byte)Marshal.SizeOf(pos);
			pos.mask = FSMI_POS_BIT.STATE | FSMI_POS_BIT.POSITION  | FSMI_POS_BIT.MAX_SPEED;
			pos.state = FSMI_State.NO_PAUSE;

			// Let's start the motion
			mi.BeginMotionControl();

			double PITCH_FACTOR = -1000; // TODO: magic number and sign to be adjusted
			double HEAVE_FACTOR = -100;  // TODO: magic number and sign to be adjusted
			double ROLL_FACTOR  = 1000;  // TODO: magic number and sign to be adjusted

			double PITCH_LOW_PASS_FILTER = 0.1;   // TODO: Adjust for better smoothness
			double ROLL_LOW_PASS_FILTER  = 0.008; // TODO: Adjust for better smoothness
			double HEAVE_LOW_PASS_FILTER = 0.1;   // TODO: Adjust for better smoothness

			double filteredPitch = 0;
			double filteredRoll  = 0;
			double filteredHeave = 0;

				for (int i = 2/* we need history for yaw*/; i < csv.Count; ++i)
			{
				var entryN2 = csv[i - 2];
				var entryN1 = csv[i - 1];
				var entryN0 = csv[i];

				var timeDiff = (entryN0[Fields.Time] - entryN1[Fields.Time]);
				if (timeDiff < 0.01)
				{
					continue;
				}

				// Calculate forward acceleration from speed and altidude
				var forwardAcc = (entryN0[Fields.Speed] - entryN1[Fields.Speed]) / timeDiff;
				var upAcc      = (entryN0[Fields.Speed] - entryN1[Fields.Speed]) / timeDiff;

				// Calculate fake lateral acceleration from yaw rate change
				var dLonA = entryN0[Fields.Lon] - entryN1[Fields.Lon]; // x
				var dLatA = entryN0[Fields.Lat] - entryN1[Fields.Lat]; // y
				var dLonB = entryN1[Fields.Lon] - entryN2[Fields.Lon]; // x
				var dLatB = entryN1[Fields.Lat] - entryN2[Fields.Lat]; // y

				var yawChange = Vector.AngleBetween(new Vector(dLonA, dLatA), new Vector(dLonB, dLatB));
				var rightAcc = yawChange * entryN0[Fields.Speed];

				// Apply smoothness
				filteredPitch += (forwardAcc * PITCH_FACTOR - filteredPitch) * PITCH_LOW_PASS_FILTER;
				filteredRoll  += (rightAcc   * ROLL_FACTOR  - filteredRoll)  * ROLL_LOW_PASS_FILTER;
				filteredHeave += (upAcc      * HEAVE_FACTOR - filteredHeave) * HEAVE_LOW_PASS_FILTER;

				// Fill demo data
				pos.pitch = (short)Clamp(-32767, filteredPitch, 32767);
				pos.roll  = (short)Clamp(-32767, filteredRoll,  32767);
				pos.heave = (short)Clamp(-32767, filteredHeave, 32767);
				pos.maxSpeed = 65535;

				mi.SendTopTablePosLog(ref pos);

				Thread.Sleep((int)(samplingInterval * 1000));
			}

			mi.EndMotionControl();
		}

	}
}

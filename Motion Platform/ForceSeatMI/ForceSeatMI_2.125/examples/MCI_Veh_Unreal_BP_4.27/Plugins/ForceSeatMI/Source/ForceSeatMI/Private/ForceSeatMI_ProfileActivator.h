/*
 * Copyright (C) 2012-2022 MotionSystems
 *
 * This file is part of ForceSeatMI SDK.
 *
 * www.motionsystems.eu
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "ForceSeatMI_Functions.h"

#include "Developer/Settings/Public/ISettingsModule.h"

namespace ProfileActivator
{
	bool ActivateProfile(FSMI_Handle handle, const char* profileName)
	{
		return ForceSeatMI_ActivateProfile(handle, profileName) != FSMI_False;
	}

	bool SetAppID(FSMI_Handle handle, const char* appId)
	{
		return ForceSeatMI_SetAppID(handle, appId) != FSMI_False;
	}

	template <class T>
	void ActivateProfileAndSetAppID(FSMI_Handle handle, const char* defaultProfile, const char* defaultAppID)
	{
		const auto settings = GetDefault<T>();

		if (settings)
		{
			if (defaultAppID && SetAppID(handle, defaultAppID))
			{
				return;
			}

			if (!settings->AppID.IsEmpty() && SetAppID(handle, TCHAR_TO_ANSI(*(settings->AppID))))
			{
				return;
			}
		}

		if (settings && settings->ActivateProfileOnStartup)
		{
			if (settings->ProfileName.IsEmpty() && defaultProfile)
			{
				ActivateProfile(handle, defaultProfile);
			}
			else
			{
				ActivateProfile(handle, TCHAR_TO_ANSI(*(settings->ProfileName)));
			}
		}
	}
}

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

#include "ForceSeatMI_Impl.h"
#include "ForceSeatMI_API.h"
#include "ForceSeatMI_ProfileActivator.h"
#include "ForceSeatMI_Settings.h"

#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "ForceSeatMI_Namespace"

namespace
{
	constexpr const char* const PROJECT      = "Project";
	constexpr const char* const PLUGINS      = "Plugins";
	constexpr const char* const SETTINGS     = "Settings";
	constexpr const char* const FSMISETTINGS = "ForceSeatMISettings";
}

ForceSeatMI_Impl::ForceSeatMI_Impl()
{
	m_handle = ForceSeatMI_Create();
}

ForceSeatMI_Impl::~ForceSeatMI_Impl()
{
	ForceSeatMI_Delete(m_handle);
	m_handle = nullptr;
}

IForceSeatMI_API* ForceSeatMI_Impl::CreateAPI(const char* profile, const char* appID)
{
	auto api = new ForceSeatMI_API(m_handle);

	ProfileActivator::ActivateProfileAndSetAppID<UForceSeatMISettings>(m_handle, profile, appID);

	return api;
}

void ForceSeatMI_Impl::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>(SETTINGS))
	{
		SettingModule->RegisterSettings(PROJECT,
			PLUGINS,
			FSMISETTINGS,
			LOCTEXT("RuntimeSettingsName", "ForceSeatMI"),
			LOCTEXT("RuntimeSettingsDescription", "Configure ForceSeatMI plugin"),
			GetMutableDefault<UForceSeatMISettings>()
		);
	}
}

void ForceSeatMI_Impl::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(SETTINGS))
	{
		SettingsModule->UnregisterSettings(PROJECT, PLUGINS, FSMISETTINGS);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(ForceSeatMI_Impl, ForceSeatMI)

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
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _MSVC_LANG
#define _MSVC_LANG 1
#endif

#include <Windows.h>
#include <stdlib.h>

#include "ForceSeatMI_Functions.h"

static HMODULE ForceSeatMI_LoadLibrarySecure();
static void    ForceSeatMI_FreeLibrarySecure(HMODULE handle);

typedef FSMI_Handle (*ForceSeatMI_Create_Func)                    ();
typedef void        (*ForceSeatMI_Delete_Func)                    (FSMI_Handle api);
typedef FSMI_Bool   (*ForceSeatMI_BeginMotionControl_Func)        (FSMI_Handle api);
typedef FSMI_Bool   (*ForceSeatMI_EndMotionControl_Func)          (FSMI_Handle api);
typedef FSMI_Bool   (*ForceSeatMI_GetPlatformInfoEx_Func)         (FSMI_Handle api, FSMI_PlatformInfo* platformInfo, FSMI_UINT32 platformInfoStructSize, FSMI_UINT32 timeout);
typedef FSMI_Bool   (*ForceSeatMI_SendTelemetry_Func)             (FSMI_Handle api, const FSMI_Telemetry* telemetry);
typedef FSMI_Bool   (*ForceSeatMI_SendTelemetry2_Func)            (FSMI_Handle api, const FSMI_Telemetry* telemetry, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects);
typedef FSMI_Bool   (*ForceSeatMI_SendTelemetryACE_Func)          (FSMI_Handle api, const FSMI_TelemetryACE* telemetry);
typedef FSMI_Bool   (*ForceSeatMI_SendTelemetryACE2_Func)         (FSMI_Handle api, const FSMI_TelemetryACE* telemetry, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects);
typedef FSMI_Bool   (*ForceSeatMI_SendTopTablePosLog_Func)        (FSMI_Handle api, const FSMI_TopTablePositionLogical* position);
typedef FSMI_Bool   (*ForceSeatMI_SendTopTablePosLog2_Func)       (FSMI_Handle api, const FSMI_TopTablePositionLogical* position, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects);
typedef FSMI_Bool   (*ForceSeatMI_SendTopTablePosPhy_Func)        (FSMI_Handle api, const FSMI_TopTablePositionPhysical* position);
typedef FSMI_Bool   (*ForceSeatMI_SendTopTablePosPhy2_Func)       (FSMI_Handle api, const FSMI_TopTablePositionPhysical* position, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects);
typedef FSMI_Bool   (*ForceSeatMI_SendTopTableMatrixPhy_Func)     (FSMI_Handle api, const FSMI_TopTableMatrixPhysical* matrix);
typedef FSMI_Bool   (*ForceSeatMI_SendTopTableMatrixPhy2_Func)    (FSMI_Handle api, const FSMI_TopTableMatrixPhysical* matrix, const FSMI_SFX* sfx, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects);
typedef FSMI_Bool   (*ForceSeatMI_SendTactileFeedbackEffects_Func)(FSMI_Handle api, const FSMI_TactileAudioBasedFeedbackEffects* audioEffects);
typedef FSMI_Bool   (*ForceSeatMI_ActivateProfile_Func)           (FSMI_Handle api, const char* profileName);
typedef FSMI_Bool   (*ForceSeatMI_SetAppID_Func)                  (FSMI_Handle api, const char* appId);
typedef FSMI_Bool   (*ForceSeatMI_Park_Func)                      (FSMI_Handle api, FSMI_UINT8 parkMode);

typedef struct ForceSeatMI_Functions
{
	ForceSeatMI_Create_Func                        Create;
	ForceSeatMI_Delete_Func                        Delete;
	ForceSeatMI_BeginMotionControl_Func            BeginMotionControl;
	ForceSeatMI_EndMotionControl_Func              EndMotionControl;
	ForceSeatMI_GetPlatformInfoEx_Func             GetPlatformInfoEx;
	ForceSeatMI_SendTelemetry_Func                 SendTelemetry;
	ForceSeatMI_SendTelemetry2_Func                SendTelemetry2;
	ForceSeatMI_SendTelemetryACE_Func              SendTelemetryACE;
	ForceSeatMI_SendTelemetryACE2_Func             SendTelemetryACE2;
	ForceSeatMI_SendTopTablePosLog_Func            SendTopTablePosLog;
	ForceSeatMI_SendTopTablePosLog2_Func           SendTopTablePosLog2;
	ForceSeatMI_SendTopTablePosPhy_Func            SendTopTablePosPhy;
	ForceSeatMI_SendTopTablePosPhy2_Func           SendTopTablePosPhy2;
	ForceSeatMI_SendTopTableMatrixPhy_Func         SendTopTableMatrixPhy;
	ForceSeatMI_SendTopTableMatrixPhy2_Func        SendTopTableMatrixPhy2;
	ForceSeatMI_SendTactileFeedbackEffects_Func    SendTactileFeedbackEffects;
	ForceSeatMI_ActivateProfile_Func               ActivateProfile;
	ForceSeatMI_SetAppID_Func                      SetAppID;
	ForceSeatMI_Park_Func                          Park;
	HMODULE                                        LibraryHandle;
	FSMI_Handle                                    API;
} ForceSeatMI_Functions;

static void ForceSeatMI_LoadLibrary(ForceSeatMI_Functions* functions)
{
	if (! functions)
	{
		return;
	}
	memset(functions, 0, sizeof(ForceSeatMI_Functions));
	functions->LibraryHandle = ForceSeatMI_LoadLibrarySecure();
	if (functions->LibraryHandle == NULL)
	{
		return;
	}

	functions->Create                     = (ForceSeatMI_Create_Func)                    GetProcAddress(functions->LibraryHandle, "ForceSeatMI_Create");
	functions->Delete                     = (ForceSeatMI_Delete_Func)                    GetProcAddress(functions->LibraryHandle, "ForceSeatMI_Delete");
	functions->BeginMotionControl         = (ForceSeatMI_BeginMotionControl_Func)        GetProcAddress(functions->LibraryHandle, "ForceSeatMI_BeginMotionControl");
	functions->EndMotionControl           = (ForceSeatMI_EndMotionControl_Func)          GetProcAddress(functions->LibraryHandle, "ForceSeatMI_EndMotionControl");
	functions->GetPlatformInfoEx          = (ForceSeatMI_GetPlatformInfoEx_Func)         GetProcAddress(functions->LibraryHandle, "ForceSeatMI_GetPlatformInfoEx");
	functions->SendTelemetry              = (ForceSeatMI_SendTelemetry_Func)             GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTelemetry");
	functions->SendTelemetry2             = (ForceSeatMI_SendTelemetry2_Func)            GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTelemetry2");
	functions->SendTelemetryACE           = (ForceSeatMI_SendTelemetryACE_Func)          GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTelemetryACE");
	functions->SendTelemetryACE2          = (ForceSeatMI_SendTelemetryACE2_Func)         GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTelemetryACE2");
	functions->SendTopTablePosLog         = (ForceSeatMI_SendTopTablePosLog_Func)        GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTopTablePosLog");
	functions->SendTopTablePosLog2        = (ForceSeatMI_SendTopTablePosLog2_Func)       GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTopTablePosLog2");
	functions->SendTopTablePosPhy         = (ForceSeatMI_SendTopTablePosPhy_Func)        GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTopTablePosPhy");
	functions->SendTopTablePosPhy2        = (ForceSeatMI_SendTopTablePosPhy2_Func)       GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTopTablePosPhy2");
	functions->SendTopTableMatrixPhy      = (ForceSeatMI_SendTopTableMatrixPhy_Func)     GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTopTableMatrixPhy");
	functions->SendTopTableMatrixPhy2     = (ForceSeatMI_SendTopTableMatrixPhy2_Func)    GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTopTableMatrixPhy2");
	functions->SendTactileFeedbackEffects = (ForceSeatMI_SendTactileFeedbackEffects_Func)GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SendTactileFeedbackEffects");
	functions->ActivateProfile            = (ForceSeatMI_ActivateProfile_Func)           GetProcAddress(functions->LibraryHandle, "ForceSeatMI_ActivateProfile");
	functions->SetAppID                   = (ForceSeatMI_SetAppID_Func)                  GetProcAddress(functions->LibraryHandle, "ForceSeatMI_SetAppID");
	functions->Park                       = (ForceSeatMI_Park_Func)                      GetProcAddress(functions->LibraryHandle, "ForceSeatMI_Park");

	if (functions->Create                     == NULL || 
		functions->Delete                     == NULL ||
		functions->BeginMotionControl         == NULL ||
		functions->EndMotionControl           == NULL ||
		functions->GetPlatformInfoEx          == NULL ||
		functions->SendTelemetry              == NULL ||
		functions->SendTelemetry2             == NULL ||
		functions->SendTopTablePosLog         == NULL ||
		functions->SendTopTablePosLog2        == NULL ||
		functions->SendTopTablePosPhy         == NULL ||
		functions->SendTopTablePosPhy2        == NULL ||
		functions->SendTopTableMatrixPhy      == NULL ||
		functions->SendTopTableMatrixPhy2     == NULL ||
		functions->SendTactileFeedbackEffects == NULL ||
		functions->ActivateProfile            == NULL ||
		functions->SetAppID                   == NULL ||
		functions->Park                       == NULL)
	{
		ForceSeatMI_FreeLibrarySecure(functions->LibraryHandle);
		memset(functions, 0, sizeof(ForceSeatMI_Functions));
	}
}

static void ForceSeatMI_FreeLibrary(ForceSeatMI_Functions* functions)
{
	if (functions)
	{
		if (functions->LibraryHandle)
		{
			FreeLibrary(functions->LibraryHandle);
		}
		memset(functions, 0, sizeof(ForceSeatMI_Functions));
	}
}

FSMI_Handle FSMI_FATTR ForceSeatMI_Create()
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)malloc(sizeof(ForceSeatMI_Functions));
	ForceSeatMI_LoadLibrary(functions);
	if (! functions || ! functions->Create)
	{
		ForceSeatMI_FreeLibrary(functions);
		free(functions); // Free memory if it was reserved
		return NULL;
	}
	functions->API = functions->Create();
	return (FSMI_Handle)functions;
}

void FSMI_FATTR ForceSeatMI_Delete(FSMI_Handle api)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->Delete)
	{
		functions->Delete(functions->API);
	}
	ForceSeatMI_FreeLibrary(functions);
	free(functions);
}

FSMI_Bool FSMI_FATTR ForceSeatMI_BeginMotionControl(FSMI_Handle api)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->BeginMotionControl)
	{
		return functions->BeginMotionControl(functions->API);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_EndMotionControl(FSMI_Handle api)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->EndMotionControl)
	{
		return functions->EndMotionControl(functions->API);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_GetPlatformInfoEx(FSMI_Handle api,
                                                   FSMI_PlatformInfo* platformInfo,
                                                   FSMI_UINT32 platformInfoStructSize, 
                                                   FSMI_UINT32 timeout)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->GetPlatformInfoEx)
	{
		return functions->GetPlatformInfoEx(functions->API, platformInfo, platformInfoStructSize, timeout);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTelemetry(FSMI_Handle api,
                                               const FSMI_Telemetry* telemetry)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTelemetry)
	{
		return functions->SendTelemetry(functions->API, telemetry);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTelemetry2(FSMI_Handle api,
                                                const FSMI_Telemetry* telemetry,
                                                const FSMI_SFX* sfx,
                                                const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTelemetry2)
	{
		return functions->SendTelemetry2(functions->API, telemetry, sfx, audioEffects);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTelemetryACE(FSMI_Handle api,
                                                  const FSMI_TelemetryACE* telemetry)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTelemetryACE)
	{
		return functions->SendTelemetryACE(functions->API, telemetry);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTelemetryACE2(FSMI_Handle api,
                                                   const FSMI_TelemetryACE* telemetry,
                                                   const FSMI_SFX* sfx,
                                                   const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTelemetryACE2)
	{
		return functions->SendTelemetryACE2(functions->API, telemetry, sfx, audioEffects);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTopTablePosLog(FSMI_Handle api,
                                                    const FSMI_TopTablePositionLogical* position)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTopTablePosLog)
	{
		return functions->SendTopTablePosLog(functions->API, position);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTopTablePosLog2(FSMI_Handle api,
                                                     const FSMI_TopTablePositionLogical* position,
                                                     const FSMI_SFX* sfx,
                                                     const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTopTablePosLog2)
	{
		return functions->SendTopTablePosLog2(functions->API, position, sfx, audioEffects);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTopTablePosPhy(FSMI_Handle api,
                                                    const FSMI_TopTablePositionPhysical* position)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTopTablePosPhy)
	{
		return functions->SendTopTablePosPhy(functions->API, position);
	}
	return FSMI_False;
}
FSMI_Bool FSMI_FATTR ForceSeatMI_SendTopTablePosPhy2(FSMI_Handle api,
                                                     const FSMI_TopTablePositionPhysical* position,
                                                     const FSMI_SFX* sfx,
                                                     const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTopTablePosPhy2)
	{
		return functions->SendTopTablePosPhy2(functions->API, position, sfx, audioEffects);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTopTableMatrixPhy(FSMI_Handle api,
                                                       const FSMI_TopTableMatrixPhysical* matrix)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTopTableMatrixPhy)
	{
		return functions->SendTopTableMatrixPhy(functions->API, matrix);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTopTableMatrixPhy2(FSMI_Handle api,
                                                        const FSMI_TopTableMatrixPhysical* matrix,
                                                        const FSMI_SFX* sfx,
                                                        const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTopTableMatrixPhy2)
	{
		return functions->SendTopTableMatrixPhy2(functions->API, matrix, sfx, audioEffects);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SendTactileFeedbackEffects(FSMI_Handle api,
                                                            const FSMI_TactileAudioBasedFeedbackEffects* audioEffects)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SendTactileFeedbackEffects)
	{
		return functions->SendTactileFeedbackEffects(functions->API, audioEffects);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_ActivateProfile(FSMI_Handle api, const char* profileName)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->ActivateProfile)
	{
		return functions->ActivateProfile(functions->API, profileName);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_SetAppID(FSMI_Handle api, const char* appId)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->SetAppID)
	{
		return functions->SetAppID(functions->API, appId);
	}
	return FSMI_False;
}

FSMI_Bool FSMI_FATTR ForceSeatMI_Park(FSMI_Handle api, FSMI_UINT8 parkMode)
{
	ForceSeatMI_Functions* functions = (ForceSeatMI_Functions*)api;
	if (functions && functions->Park)
	{
		return functions->Park(functions->API, parkMode);
	}
	return FSMI_False;
}

HMODULE ForceSeatMI_LoadLibrarySecure()
{
#ifdef _M_X64
	static const wchar_t* const LIBRARY_DLL_NAME = L"ForceSeatMI64.dll";
	static const wchar_t* const REG_PATH         = L"SOFTWARE\\Wow6432Node\\MotionSystems\\ForceSeatPM";
#else
	static const wchar_t* const LIBRARY_DLL_NAME = L"ForceSeatMI32.dll";
	static const wchar_t* const REG_PATH         = L"SOFTWARE\\MotionSystems\\ForceSeatPM";
#endif

	HKEY hKey  = NULL;
	HMODULE hLibrary = NULL;

	// Let's check if there is ForceSeatPM installed, if yes there is ForceSeatMIxx.dll that can be used
	if (ERROR_SUCCESS == RegOpenKeyW(HKEY_LOCAL_MACHINE, REG_PATH, &hKey))
	{
		wchar_t buffer[MAX_PATH * 2] = {0};
		DWORD dwBufSize = sizeof(buffer);
		DWORD dwType = REG_SZ;

		if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"InstallationPath", 0, &dwType, (BYTE*)buffer, &dwBufSize))
		{
			wcscat_s(buffer, MAX_PATH * 2, L"\\");
			wcscat_s(buffer, MAX_PATH * 2, LIBRARY_DLL_NAME);
			hLibrary = LoadLibraryW(buffer);
		}

		RegCloseKey(hKey);
	}

	// If there is still not ForceSeatMIxx.dll found, then let's try in standard search path
	if (hLibrary == NULL)
	{
		hLibrary = LoadLibraryW(LIBRARY_DLL_NAME);
	}

	return hLibrary;
}

void ForceSeatMI_FreeLibrarySecure(HMODULE handle)
{
	FreeLibrary(handle);
}
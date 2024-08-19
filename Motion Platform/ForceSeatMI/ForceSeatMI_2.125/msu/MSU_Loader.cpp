/*
 * Copyright (C) 2012-2022 MotionSystems
 * 
 * This file is part MSU SDK.
 *
 * www.motionsystems.eu
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */ 
#include <Windows.h>

#include "MSU_GyroPlayer.h"

static HMODULE MSU_LoadLibrarySecure();

namespace
{
	typedef MSU_GP_Handle (*MSU_GyroPlayer_Create_Func)();
	typedef void (*MSU_GyroPlayer_Delete_Func)(MSU_GP_Handle api);

	typedef void (*MSU_GyroPlayer_SetCsvSkipLines_Func)(MSU_GP_Handle api, unsigned int count);
	typedef void (*MSU_GyroPlayer_ClearCsvMapping_Func)(MSU_GP_Handle api);
	typedef void (*MSU_GyroPlayer_SetCsvMapping_Func)(MSU_GP_Handle api, unsigned int col, MSU_GyroPlayerRole role, MSU_GyroPlayerMappingInversion inversion, float multiplier);

	typedef MSU_GyroPlayerErrorCode (*MSU_GyroPlayer_LoadCsv_Func)(MSU_GP_Handle api, const wchar_t* filename, char delimiter, unsigned int sampleInterval_ms, MSU_GyroPlayerTimeUnit timeUnit);
	typedef MSU_GyroPlayerErrorCode (*MSU_GyroPlayer_LoadCsv2_Func)(MSU_GP_Handle api, const wchar_t* filename, char delimiter, uint64_t sampleInterval_us, MSU_GyroPlayerTimeUnit timeUnit);
	typedef MSU_GyroPlayerErrorCode (*MSU_GyroPlayer_LoadCsvEx_Func)(MSU_GP_Handle api, const wchar_t* filename, char delimiter);
	typedef MSU_GyroPlayerErrorCode (*MSU_GyroPlayer_PlayAsync_Func)(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, void* userData);
	typedef MSU_GyroPlayerErrorCode (*MSU_GyroPlayer_PlayAsyncEx_Func)(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, MSY_GyroPlayerFinishCallback finishCallback, void* userData);
	typedef MSU_GyroPlayerErrorCode (*MSU_GyroPlayer_Dump_Func)(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, MSY_GyroPlayerFinishCallback finishCallback, void* userData);

	typedef bool (*MSU_GyroPlayer_IsFinished_Func)(MSU_GP_Handle api);
	typedef bool (*MSU_GyroPlayer_IsPaused_Func)(MSU_GP_Handle api);
	typedef bool (*MSU_GyroPlayer_IsStopped_Func)(MSU_GP_Handle api);
	typedef bool (*MSU_GyroPlayer_IsPlaying_Func)(MSU_GP_Handle api);
	typedef void (*MSU_GyroPlayer_Wait_Func)(MSU_GP_Handle api);
	typedef int  (*MSU_GyroPlayer_GetEntryCount_Func)(MSU_GP_Handle api);
	typedef void (*MSU_GyroPlayer_Stop_Func)(MSU_GP_Handle api);
	typedef void (*MSU_GyroPlayer_Pause_Func)(MSU_GP_Handle api);

	struct MSU_PlayerFunctions
	{
		MSU_GyroPlayer_Create_Func          pCreate;
		MSU_GyroPlayer_Delete_Func          pDelete;
		MSU_GyroPlayer_SetCsvSkipLines_Func pSetCsvSkipLines;
		MSU_GyroPlayer_ClearCsvMapping_Func pClearCsvMapping;
		MSU_GyroPlayer_SetCsvMapping_Func   pSetCsvMapping;

		MSU_GyroPlayer_LoadCsv_Func         pLoadCsv;
		MSU_GyroPlayer_LoadCsv2_Func        pLoadCsv2;
		MSU_GyroPlayer_LoadCsvEx_Func       pLoadCsvEx;
		MSU_GyroPlayer_PlayAsync_Func       pPlayAsync;
		MSU_GyroPlayer_PlayAsyncEx_Func     pPlayAsyncEx;
		MSU_GyroPlayer_Dump_Func            pDump;

		MSU_GyroPlayer_IsFinished_Func      pIsFinished;
		MSU_GyroPlayer_IsPaused_Func        pIsPaused;
		MSU_GyroPlayer_IsStopped_Func       pIsStopped;
		MSU_GyroPlayer_IsPlaying_Func       pIsPlaying;
		MSU_GyroPlayer_Wait_Func            pWait;
		MSU_GyroPlayer_GetEntryCount_Func   pEntryCount;
		MSU_GyroPlayer_Stop_Func            pStop;
		MSU_GyroPlayer_Pause_Func           pPause;

		HMODULE           libraryHandle;
		MSU_GP_Handle     api;

		template <class T>
		bool Load(T& field, const char* name)
		{
			field = reinterpret_cast<T>(GetProcAddress(libraryHandle, name));
			return field != nullptr;
		}
	};

	static void MSU_LoadPlayerFunctions(MSU_PlayerFunctions* functions)
	{
		if (! functions)
		{
			return;
		}

		memset(functions, 0, sizeof(MSU_PlayerFunctions));
		functions->libraryHandle = MSU_LoadLibrarySecure();

		if (functions->libraryHandle == nullptr)
		{
			return;
		}

		bool ok = true;
		ok = ok && functions->Load(functions->pCreate,          "MSU_GyroPlayer_Create");
		ok = ok && functions->Load(functions->pDelete,          "MSU_GyroPlayer_Delete");
		ok = ok && functions->Load(functions->pSetCsvSkipLines, "MSU_GyroPlayer_SetCsvSkipLines");
		ok = ok && functions->Load(functions->pClearCsvMapping, "MSU_GyroPlayer_ClearCsvMapping");
		ok = ok && functions->Load(functions->pSetCsvMapping,   "MSU_GyroPlayer_SetCsvMapping");
		ok = ok && functions->Load(functions->pLoadCsv,         "MSU_GyroPlayer_LoadCsv");
		ok = ok && functions->Load(functions->pLoadCsv2,        "MSU_GyroPlayer_LoadCsv2");
		ok = ok && functions->Load(functions->pLoadCsvEx,       "MSU_GyroPlayer_LoadCsvEx");
		ok = ok && functions->Load(functions->pPlayAsync,       "MSU_GyroPlayer_PlayAsync");
		ok = ok && functions->Load(functions->pPlayAsyncEx,     "MSU_GyroPlayer_PlayAsyncEx");
		ok = ok && functions->Load(functions->pDump,            "MSU_GyroPlayer_Dump");
		ok = ok && functions->Load(functions->pIsFinished,      "MSU_GyroPlayer_IsFinished");
		ok = ok && functions->Load(functions->pIsPaused,        "MSU_GyroPlayer_IsPaused");
		ok = ok && functions->Load(functions->pIsStopped,       "MSU_GyroPlayer_IsStopped");
		ok = ok && functions->Load(functions->pIsPlaying,       "MSU_GyroPlayer_IsPlaying");
		ok = ok && functions->Load(functions->pWait,            "MSU_GyroPlayer_Wait");
		ok = ok && functions->Load(functions->pEntryCount,      "MSU_GyroPlayer_GetEntryCount");
		ok = ok && functions->Load(functions->pStop,            "MSU_GyroPlayer_Stop");
		ok = ok && functions->Load(functions->pPause,           "MSU_GyroPlayer_Pause");

		if (! ok)
		{
			::FreeLibrary(functions->libraryHandle);
			memset(functions, 0, sizeof(MSU_PlayerFunctions));
		}
	}

	template <class T>
	static void MSU_ReleaseLibrary(T* functions)
	{
		if (functions)
		{
			if (functions->libraryHandle)
			{
				::FreeLibrary(functions->libraryHandle);
			}
			memset(functions, 0, sizeof(T));
		}
	}
}

extern "C" MSU_GP_Handle __cdecl MSU_GyroPlayer_Create()
{
	auto functions = new MSU_PlayerFunctions;
	MSU_LoadPlayerFunctions(functions);
	if (! functions || ! functions->pCreate)
	{
		MSU_ReleaseLibrary(functions);
		delete functions; // Free memory if it was reserved
		return nullptr;
	}
	functions->api = functions->pCreate();
	return reinterpret_cast<MSU_GP_Handle>(functions);
}

extern "C" void __cdecl MSU_GyroPlayer_Delete(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pDelete)
	{
		functions->pDelete(functions->api);
	}
	MSU_ReleaseLibrary(functions);
	delete functions;
}

extern "C"  void __cdecl MSU_GyroPlayer_SetCsvSkipLines(MSU_GP_Handle api, unsigned int count)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pSetCsvSkipLines)
	{
		functions->pSetCsvSkipLines(functions->api, count);
	}
}

extern "C"  void __cdecl MSU_GyroPlayer_ClearCsvMapping(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pClearCsvMapping)
	{
		functions->pClearCsvMapping(functions->api);
	}
}

extern "C"  void __cdecl MSU_GyroPlayer_SetCsvMapping(MSU_GP_Handle api, unsigned int col, MSU_GyroPlayerRole role, MSU_GyroPlayerMappingInversion inversion, float multiplier)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pSetCsvMapping)
	{
		functions->pSetCsvMapping(functions->api, col, role, inversion, multiplier);
	}
}

extern "C"  MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_LoadCsv(MSU_GP_Handle api, const wchar_t* filename, char delimiter, unsigned int sampleInterval_ms, MSU_GyroPlayerTimeUnit timeUnit)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pLoadCsv)
	{
		return functions->pLoadCsv(functions->api, filename, delimiter, sampleInterval_ms, timeUnit);
	}
	return MSU_GPEC_InvalidPointer;
}

extern "C"  MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_LoadCsv2(MSU_GP_Handle api, const wchar_t* filename, char delimiter, uint64_t sampleInterval_us, MSU_GyroPlayerTimeUnit timeUnit)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pLoadCsv2)
	{
		return functions->pLoadCsv2(functions->api, filename, delimiter, sampleInterval_us, timeUnit);
	}
	return MSU_GPEC_InvalidPointer;
}

extern "C"  MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_LoadCsvEx(MSU_GP_Handle api, const wchar_t* filename, char delimiter)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pLoadCsvEx)
	{
		return functions->pLoadCsvEx(functions->api, filename, delimiter);
	}
	return MSU_GPEC_InvalidPointer;
}

extern "C"  MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_PlayAsync(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, void* userData)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pPlayAsync)
	{
		return functions->pPlayAsync(functions->api, callback, userData);
	}
	return MSU_GPEC_InvalidPointer;
}

extern "C"  MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_PlayAsyncEx(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, MSY_GyroPlayerFinishCallback finishCallback, void* userData)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pPlayAsyncEx)
	{
		return functions->pPlayAsyncEx(functions->api, callback, finishCallback, userData);
	}
	return MSU_GPEC_InvalidPointer;
}

extern "C"  MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_Dump(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, MSY_GyroPlayerFinishCallback finishCallback, void* userData)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pDump)
	{
		return functions->pDump(functions->api, callback, finishCallback, userData);
	}
	return MSU_GPEC_InvalidPointer;
}

extern "C"  bool __cdecl MSU_GyroPlayer_IsFinished(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pIsFinished)
	{
		return functions->pIsFinished(functions->api);
	}
	return false;
}

extern "C"  bool __cdecl MSU_GyroPlayer_IsPaused(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pIsPaused)
	{
		return functions->pIsPaused(functions->api);
	}
	return false;
}

extern "C"  bool __cdecl MSU_GyroPlayer_IsStopped(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pIsStopped)
	{
		return functions->pIsStopped(functions->api);
	}
	return false;
}

extern "C"  bool __cdecl MSU_GyroPlayer_IsPlaying(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pIsPlaying)
	{
		return functions->pIsPlaying(functions->api);
	}
	return false;
}

extern "C"  void __cdecl MSU_GyroPlayer_Wait(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pWait)
	{
		functions->pWait(functions->api);
	}
}

extern "C"  int __cdecl MSU_GyroPlayer_GetEntryCount(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pEntryCount)
	{
		return functions->pEntryCount(functions->api);
	}
	return -1;
}

extern "C"  void __cdecl MSU_GyroPlayer_Stop(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pStop)
	{
		functions->pStop(functions->api);
	}
}

extern "C"  void __cdecl MSU_GyroPlayer_Pause(MSU_GP_Handle api)
{
	auto functions = reinterpret_cast<MSU_PlayerFunctions*>(api);
	if (functions && functions->pPause)
	{
		functions->pPause(functions->api);
	}
}

static HMODULE MSU_LoadLibrarySecure()
{
#ifdef _M_X64
	static const wchar_t* const LIBRARY_DLL_NAME = L"MSU64.dll";
	static const wchar_t* const REG_PATH = L"SOFTWARE\\Wow6432Node\\MotionSystems\\ForceSeatPM";
#else
	static const wchar_t* const LIBRARY_DLL_NAME = L"MSU32.dll";
	static const wchar_t* const REG_PATH = L"SOFTWARE\\MotionSystems\\ForceSeatPM";
#endif

	HKEY hKey = NULL;
	HMODULE hLibrary = LoadLibraryW(LIBRARY_DLL_NAME);

	// Let's check if there is ForceSeatPM installed, if yes there is MSUxx.dll that can be used
	if (nullptr == hLibrary && ERROR_SUCCESS == RegOpenKeyW(HKEY_LOCAL_MACHINE, REG_PATH, &hKey))
	{
		wchar_t buffer[MAX_PATH * 2] = { 0 };
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

	return hLibrary;
}

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
#pragma once

#include "MSU_Defines.h"
#include "MSU_GyroPlayer_Common.h"

#ifdef __cplusplus
extern "C" {
#endif
	struct __MSU_GyroPlayerHandleType { int unused; };
	typedef struct __MSU_GyroPlayerHandleType* MSU_GP_Handle;

	MSU_EXPORT MSU_GP_Handle __cdecl MSU_GyroPlayer_Create();
	MSU_EXPORT void __cdecl MSU_GyroPlayer_Delete(MSU_GP_Handle api);

	MSU_EXPORT void __cdecl MSU_GyroPlayer_SetCsvSkipLines(MSU_GP_Handle api, unsigned int count);
	MSU_EXPORT void __cdecl MSU_GyroPlayer_ClearCsvMapping(MSU_GP_Handle api);
	MSU_EXPORT void __cdecl MSU_GyroPlayer_SetCsvMapping(MSU_GP_Handle api, unsigned int col, MSU_GyroPlayerRole role, MSU_GyroPlayerMappingInversion inversion, float multiplier);

	MSU_EXPORT MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_LoadCsv(MSU_GP_Handle api, const wchar_t* filename, char delimiter, unsigned int sampleInterval_ms, MSU_GyroPlayerTimeUnit timeUnit);
	MSU_EXPORT MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_LoadCsv2(MSU_GP_Handle api, const wchar_t* filename, char delimiter, uint64_t sampleInterval_us, MSU_GyroPlayerTimeUnit timeUnit);
	MSU_EXPORT MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_LoadCsvEx(MSU_GP_Handle api, const wchar_t* filename, char delimiter);
	MSU_EXPORT MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_PlayAsync(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, void* userData);
	MSU_EXPORT MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_PlayAsyncEx(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, MSY_GyroPlayerFinishCallback finishCallback, void* userData);
	MSU_EXPORT MSU_GyroPlayerErrorCode __cdecl MSU_GyroPlayer_Dump(MSU_GP_Handle api, MSY_GyroPlayerCallback callback, MSY_GyroPlayerFinishCallback finishCallback, void* userData);

	MSU_EXPORT bool __cdecl MSU_GyroPlayer_IsFinished(MSU_GP_Handle api);
	MSU_EXPORT bool __cdecl MSU_GyroPlayer_IsPaused(MSU_GP_Handle api);
	MSU_EXPORT bool __cdecl MSU_GyroPlayer_IsStopped(MSU_GP_Handle api);
	MSU_EXPORT bool __cdecl MSU_GyroPlayer_IsPlaying(MSU_GP_Handle api);
	MSU_EXPORT void __cdecl MSU_GyroPlayer_Wait(MSU_GP_Handle api);

	MSU_EXPORT int __cdecl MSU_GyroPlayer_GetEntryCount(MSU_GP_Handle api);

	MSU_EXPORT void __cdecl MSU_GyroPlayer_Stop(MSU_GP_Handle api);
	MSU_EXPORT void __cdecl MSU_GyroPlayer_Pause(MSU_GP_Handle api);

#ifdef __cplusplus
}
#endif

/*
 * Copyright (C) 2012-2020 Motion Systems
 * 
 * This file is part of ForceSeat motion system.
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
#ifndef FORCE_SEAT_DI_DEFINES_H
#define FORCE_SEAT_DI_DEFINES_H

typedef unsigned char      FSDI_UINT8;
typedef signed   char      FSDI_INT8;
typedef unsigned int       FSDI_UINT32;
typedef signed   int       FSDI_INT32;
typedef unsigned short     FSDI_UINT16;
typedef signed   short     FSDI_INT16;
typedef float              FSDI_FLOAT;
typedef unsigned long long FSDI_UINT64;
typedef char               FSDI_Bool;
typedef char               FSDI_Char;

enum FSDI_Defines
{
	FSDI_True                     = 1,
	FSDI_False                    = 0,
	FSDI_MotorsCount              = 6,
	FSDI_SerialNumberBytesLength  = 12,
	FSDI_SerialNumberStringLength = 28,
	FSDI_SFX_MaxEffectsCount      = 4
};

#ifdef EXPORT_FORCESEAT_DI
	#if defined(_WIN32)
		#define FORCESEAT_DI_EXPORT __declspec(dllexport)
	#else
		#define FORCESEAT_DI_EXPORT __attribute__((visibility("default")))
		#define __cdecl
	#endif
#elif defined(NO_IMPORT_FORCESEAT_DI)
	#if defined(_WIN32)
		#define FORCESEAT_DI_EXPORT
	#else
		#define FORCESEAT_DI_EXPORT
		#define __cdecl
	#endif
#else
	#if defined(_WIN32)
		#define FORCESEAT_DI_EXPORT __declspec(dllimport)
	#else
		#define FORCESEAT_DI_EXPORT
		#define __cdecl
	#endif
#endif

#if defined(_WIN32)
	#define FSDI_PACKED
#else
	#define FSDI_PACKED __attribute__((packed, aligned(1)))
#endif

#endif

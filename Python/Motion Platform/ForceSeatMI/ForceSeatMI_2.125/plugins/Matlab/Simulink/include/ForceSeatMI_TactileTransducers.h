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
#ifndef FORCE_SEAT_MI_TACTILE_TRANSDUCERS_H
#define FORCE_SEAT_MI_TACTILE_TRANSDUCERS_H

#include "ForceSeatMI_Common.h"

#pragma pack(push, 1)

/*
 * Definition of available streams for tactile feedback system
 */
typedef enum FSMI_TactileTransducersStream
{
	FSMI_StreamCenter,
	FSMI_StreamEngine,
	FSMI_StreamFL,
	FSMI_StreamFR,
	FSMI_StreamRL,
	FSMI_StreamRR,
	FSMI_StreamCH,

	FSMI_StreamsCount,
} FSMI_TactileTransducersStream;

/*
 * This structure defines effects for tactile transducers. Signals of given frequency and amplitude are generates on associated audio outputs.
 */
typedef struct FSMI_TactileAudioBasedFeedbackEffects
{
	FSMI_UINT8  structSize; // put here sizeof(FSMI_TactileAudioBasedFeedbackEffects)

	FSMI_UINT32 frequency[FSMI_StreamsCount]; // frequency in Hz
	FSMI_FLOAT  amplitude[FSMI_StreamsCount]; // signal amplitude from 0 to 1
} FSMI_PACKED FSMI_TactileAudioBasedFeedbackEffects;

#pragma pack(pop)

#endif

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
#pragma once

#include <stdint.h>

static constexpr const uint32_t MAGIC_HEADER = 0x12345678;
static constexpr const uint32_t MAGIC_FOOTER = 0xABCDEF99;

#pragma pack(push, 1)

/*
 * This is an example UDP frame. Modify it for your needs.
 */
typedef struct ExampleFrame
{
	uint32_t structSize; 
	uint32_t magicHeader; // e.g.0x12345678
	
	float    roll;
	float    pitch;
	float    yaw;
	float    heave;
	float    sway;
	float    surge;

	uint32_t magicFooter; // e.g.0xABCDEF99
} 
#if defined(__GNUC__) 
	__attribute__((packed, aligned(1)))
#endif
ExampleFrame;

#pragma pack(pop)

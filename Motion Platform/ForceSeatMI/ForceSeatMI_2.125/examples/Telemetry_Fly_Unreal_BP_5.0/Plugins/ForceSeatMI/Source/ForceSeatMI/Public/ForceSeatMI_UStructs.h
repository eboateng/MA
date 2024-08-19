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

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ForceSeatMI_UStructs.generated.h"

USTRUCT(BlueprintType)
struct FTopTablePositionPhysicalBP
{
	GENERATED_BODY()

	// in radians, roll  < 0 = left,  roll > 0  = right
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Roll;

	// in radians, pitch < 0 = front, pitch > 0 = rear
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pitch;

	// in radians, yaw   < 0 = right, yaw > 0   = left
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Yaw;

	// in mm, heave < 0 - down, heave > 0 - top
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Heave;

	// in mm, sway  < 0 - left, sway  > 0 - right
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Sway;

	// in mm, surge < 0 - rear, surge > 0 - front
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Surge;

	// 0 to 64k, actual speed is not always equal to max speed due to ramps.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Paused;

	FTopTablePositionPhysicalBP()
		: Roll(0.0f)
		, Pitch(0.0f)
		, Yaw(0.0f)
		, Heave(0.0f)
		, Sway(0.0f)
		, Surge(0.0f)
		, MaxSpeed(32000)
		, Paused(false)
	{
	}
};

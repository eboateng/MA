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

#include "Engine.h"
#include "UObject/NoExportTypes.h"
#include "ForceSeatMI_Settings.generated.h"

UCLASS(config = ForceSeatMISettings)
class UForceSeatMISettings : public UObject
{
	GENERATED_BODY()
public:
	UForceSeatMISettings(const FObjectInitializer& obj);

	UPROPERTY(Config, EditAnywhere, Category = Settings)
	bool ActivateProfileOnStartup;

	UPROPERTY(Config, EditAnywhere, Category = Settings, meta = (EditCondition = "ActivateProfileOnStartup"))
	FString ProfileName;

	UPROPERTY(Config, EditAnywhere, Category = Settings)
	FString AppID;
};

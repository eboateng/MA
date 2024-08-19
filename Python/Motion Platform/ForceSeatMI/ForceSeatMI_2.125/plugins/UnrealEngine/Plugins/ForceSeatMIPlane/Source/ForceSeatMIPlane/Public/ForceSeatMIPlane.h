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

#include "Modules/ModuleManager.h"

class APawn;
class IForceSeatMIPlane_ControllerCore;

class ForceSeatMIPlane : public IModuleInterface
{
public:
	virtual IForceSeatMIPlane_ControllerCore* CreateControllerCore(APawn* plane) = 0;

public:
	static inline ForceSeatMIPlane& Get()
	{
		return FModuleManager::LoadModuleChecked<ForceSeatMIPlane>("ForceSeatMIPlane");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ForceSeatMIPlane");
	}
};

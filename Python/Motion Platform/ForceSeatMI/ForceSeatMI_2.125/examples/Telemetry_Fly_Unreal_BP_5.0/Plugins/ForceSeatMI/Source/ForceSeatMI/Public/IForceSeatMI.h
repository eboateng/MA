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

class IForceSeatMI_API;

class IForceSeatMI : public IModuleInterface
{
public:
	virtual IForceSeatMI_API* CreateAPI(const char* profile = nullptr, const char* appID = nullptr) = 0;

public:
	static inline IForceSeatMI& Get()
	{
		return FModuleManager::LoadModuleChecked<IForceSeatMI>("ForceSeatMI");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ForceSeatMI");
	}
};

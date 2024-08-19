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

#include "IForceSeatMI.h"
#include "ForceSeatMI_Functions.h"

class IForceSeatMI_API;

class ForceSeatMI_Impl : public IForceSeatMI
{
	explicit ForceSeatMI_Impl(const ForceSeatMI_Impl& other) = delete;
	explicit ForceSeatMI_Impl(const ForceSeatMI_Impl&& other) = delete;

public:
	ForceSeatMI_Impl();
	virtual ~ForceSeatMI_Impl();

public:
	virtual IForceSeatMI_API* CreateAPI(const char* profile = nullptr, const char* appID = nullptr) override;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FSMI_Handle m_handle;
};

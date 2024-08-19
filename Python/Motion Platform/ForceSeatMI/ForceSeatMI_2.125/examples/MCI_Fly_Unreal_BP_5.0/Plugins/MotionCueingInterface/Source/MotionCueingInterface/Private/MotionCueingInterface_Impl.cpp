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

#include "MotionCueingInterface_Impl.h"
#include "ForceSeatMI_MotionHost.h"

#define LOCTEXT_NAMESPACE "FMotionCueingInterfaceModule"

MotionHost* FMotionCueingInterface_Impl::CreateMotionHost(const char* profile, const char* appID)
{
	return new ForceSeatMI_MotionHost(profile, appID);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMotionCueingInterface_Impl, MotionCueingInterface)

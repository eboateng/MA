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

#include "ForceSeatMIPhysXVehicle.h"
#include "ForceSeatMIPhysXVehicle_ControllerCore.h"

#define LOCTEXT_NAMESPACE "ForceSeatMIPhysXVehicle"

class ForceSeatMIPhysXVehicle_Impl : public  ForceSeatMIPhysXVehicle
{
public:
	virtual IForceSeatMIPhysXVehicle_ControllerCore* CreateControllerCore(AWheeledVehicle* vehicle) override
	{
		return new ForceSeatMIPhysXVehicle_ControllerCore(vehicle);
	}
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(ForceSeatMIPhysXVehicle_Impl, ForceSeatMIPhysXVehicle)

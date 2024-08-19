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

#include "ForceSeatMIPhysXVehicle_Controller.h"
#include "ForceSeatMIPhysXVehicle_ControllerCore.h"
#include "IForceSeatMI.h"

#include "WheeledVehicleMovementComponent.h"
#include "WheeledVehicle.h"

UForceSeatMIPhysXVehicle_Controller::UForceSeatMIPhysXVehicle_Controller()
	: m_controller(nullptr)
	, m_source(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
}

UForceSeatMIPhysXVehicle_Controller::~UForceSeatMIPhysXVehicle_Controller()
{
	delete m_controller;
	m_controller = nullptr;
}

void UForceSeatMIPhysXVehicle_Controller::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_controller)
	{
		m_controller->Update(DeltaTime);
	}
}

void UForceSeatMIPhysXVehicle_Controller::SetVehicleObject(AWheeledVehicle* Input)
{
	if (!m_controller || m_source != Input)
	{
		m_source = Input;

		delete m_controller;

		m_controller = new ForceSeatMIPhysXVehicle_ControllerCore(Input);
		m_controller->Begin();
	}
}

void UForceSeatMIPhysXVehicle_Controller::Pause(bool paused)
{
	if (m_controller)
	{
		m_controller->Pause(paused);
	}
}

void UForceSeatMIPhysXVehicle_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (m_controller)
	{
		m_controller->Begin();
	}
}

void UForceSeatMIPhysXVehicle_Controller::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_controller)
	{
		m_controller->End();
	}
}

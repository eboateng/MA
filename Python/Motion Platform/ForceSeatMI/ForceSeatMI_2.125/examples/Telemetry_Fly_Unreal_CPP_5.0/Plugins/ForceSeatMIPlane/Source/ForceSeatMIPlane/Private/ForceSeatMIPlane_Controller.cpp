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

#include "ForceSeatMIPlane_Controller.h"
#include "ForceSeatMIPlane.h"

UForceSeatMIPlane_Controller::UForceSeatMIPlane_Controller()
	: m_controller(nullptr)
	, m_source(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
}

UForceSeatMIPlane_Controller::~UForceSeatMIPlane_Controller()
{
	delete m_controller;
	m_controller = nullptr;
}

void UForceSeatMIPlane_Controller::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_controller)
	{
		m_controller->Update(DeltaTime);
	}
}

void UForceSeatMIPlane_Controller::SetPlaneObject(APawn* Input)
{
	if (!m_controller || m_source != Input)
	{
		m_source = Input;

		delete m_controller;

		m_controller = ForceSeatMIPlane::Get().CreateControllerCore(Input);
		m_controller->Begin();
	}
}

void UForceSeatMIPlane_Controller::Pause(bool paused)
{
	if (m_controller)
	{
		m_controller->Pause(paused);
	}
}

void UForceSeatMIPlane_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (m_controller)
	{
		m_controller->Begin();
	}
}

void UForceSeatMIPlane_Controller::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_controller)
	{
		m_controller->End();
	}
}

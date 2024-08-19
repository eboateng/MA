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

class IForceSeatMIPlane_ControllerCore
{
public:
	struct ExtraTransformation
	{
		float yaw;
		float pitch;
		float roll;
		float right;
		float up;
		float forward;
	};

public:
	virtual ~IForceSeatMIPlane_ControllerCore() {}

public:
	virtual void Begin() = 0;
	virtual void End() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Pause(bool paused) = 0;
	virtual void AddExtraTransformation(const ExtraTransformation& transformation) = 0;
};

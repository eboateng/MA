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

using MotionSystems;
using System;
using UnityEngine;

public class CarController : MonoBehaviour
{
    public WheelCollider[] m_WheelColliders = new WheelCollider[4];
    public GameObject[] m_WheelMeshes = new GameObject[4];

    // Torque that will be applied to the wheels
    public float m_Torque;

    // Reverse torque used to brake
    public float m_ReverseTorque;

    // Torque for hand braking
    public float m_MaxHandbrakeTorque;

    // Maximum steer angle for the front wheels
    public float m_MaximumSteerAngle;

    // Force that puts pressure on the car
    public float m_Downforce;

    // Maximum speed that car can move with
    public float m_TopSpeed; 

    // Torque used for braking
    public float m_BrakeTorque;

    // Number of available gears
    private int m_NumberOfGears = 5;

    // Current gear
    private int m_CurrentGearNumber;

    // Vehicle body object
    private Rigidbody m_Rigidbody;

    // ForceSeatMI API
    private ForceSeatMI_Unity m_Api;
    private ForceSeatMI_Vehicle m_vehicle;
    private ForceSeatMI_Unity.ExtraParameters m_extraParameters;

    private void Start()
    {
        m_Rigidbody = GetComponent<Rigidbody>();

        // ForceSeatMI - BEGIN
        m_Api             = new ForceSeatMI_Unity();
        m_vehicle         = new ForceSeatMI_Vehicle(m_Rigidbody);
        m_extraParameters = new ForceSeatMI_Unity.ExtraParameters();

        m_vehicle.SetGearNumber(m_CurrentGearNumber);

        m_Api.SetAppID(""); // If you have dedicated app id, remove ActivateProfile calls from your code
        m_Api.ActivateProfile("SDK - Vehicle Telemetry ACE");
        m_Api.SetTelemetryObject(m_vehicle);
        m_Api.Pause(false);
        m_Api.Begin();
        // ForceSeatMI - END
    }

    private void OnDestroy()
    {
        // ForceSeatMI - BEGIN
        m_Api.End();
        // ForceSeatMI - END
    }

    private void FixedUpdate()
    {
        // Get user's input
        float h         = Input.GetAxis("Horizontal");
        float v         = Input.GetAxis("Vertical");
        float handbrake = Input.GetAxis("Jump");

        // Process input
        Move(h, v, v, handbrake);
    }

    private void Move(float steering, float accel, float footbrake, float handbrake)
    {
        RotateWheels();
        SteerWheels(steering);
        ApplyDrive(accel, footbrake, handbrake);
        CapSpeed();        
        ChangeGear();
        AddDownForce();

        // ForceSeatMI - BEGIN
        if (m_vehicle != null && m_Api != null)
        {
            m_vehicle.SetGearNumber(m_CurrentGearNumber);

            // Use extra parameters to generate custom effects, for exmp. vibrations. They will NOT be
            // filtered, smoothed or processed in any way.
            m_extraParameters.yaw     = 0;
            m_extraParameters.pitch   = 0;
            m_extraParameters.roll    = 0; // Vibration: (float)Math.Sin(Time.fixedTime * 1000) * 0.02f;
            m_extraParameters.right   = 0;
            m_extraParameters.up      = 0;
            m_extraParameters.forward = 0;

            m_Api.AddExtra(m_extraParameters);
            m_Api.Update(Time.fixedDeltaTime);
        }
        // ForceSeatMI - END
    }

    private void ChangeGear()
    {
        float f = Mathf.Abs(m_Rigidbody.velocity.magnitude * 3.6f / m_TopSpeed);
        float upgearlimit = (1 / (float)m_NumberOfGears) * (m_CurrentGearNumber + 1);
        float downgearlimit = (1 / (float)m_NumberOfGears) * m_CurrentGearNumber;

        if (m_CurrentGearNumber > 0 && f < downgearlimit)
        {
            --m_CurrentGearNumber;
        }

        if (f > upgearlimit && (m_CurrentGearNumber < (m_NumberOfGears - 1)))
        {
            ++m_CurrentGearNumber;
        }
    }

    private void RotateWheels()
    {
        Quaternion quat;
        Vector3 position;

        for (int i = 0; i < m_WheelColliders.Length; ++i)
        {
            m_WheelColliders[i].GetWorldPose(out position, out quat);
            m_WheelMeshes[i].transform.position = position;
            m_WheelMeshes[i].transform.rotation = quat;
        }
    }

    private void SteerWheels(float steering)
    {
        var angle = Mathf.Clamp(steering, -1, 1) * m_MaximumSteerAngle;

        m_WheelColliders[0].steerAngle = angle;
        m_WheelColliders[1].steerAngle = angle;
    }

    private void CapSpeed()
    {
        float currentSpeed = m_Rigidbody.velocity.magnitude * 3.6f;

        if (currentSpeed > m_TopSpeed)
        {
            m_Rigidbody.velocity = (m_TopSpeed / 3.6f) * m_Rigidbody.velocity.normalized;
        }
    }

    private void ApplyDrive(float accel, float footbrake, float handbrake)
    {
        var footbrakeTorque = -Mathf.Clamp(footbrake, -1, 0) * m_MaxHandbrakeTorque * 100;
        var handbrakeTorque =  Mathf.Clamp(handbrake, 0, 1)  * m_MaxHandbrakeTorque;
        var thrustTorque    = Mathf.Clamp(accel, 0, 1)       * m_Torque;

        if (footbrakeTorque > 0)
        {
            m_WheelColliders[0].brakeTorque = footbrakeTorque;
            m_WheelColliders[1].brakeTorque = footbrakeTorque;
            m_WheelColliders[2].brakeTorque = footbrakeTorque / 3;
            m_WheelColliders[3].brakeTorque = footbrakeTorque / 3;
        }
        else
        {
            m_WheelColliders[0].brakeTorque = 0;
            m_WheelColliders[1].brakeTorque = 0;
            m_WheelColliders[2].brakeTorque = 0;
            m_WheelColliders[3].brakeTorque = 0;
        }

        m_WheelColliders[0].motorTorque = thrustTorque / 3;
        m_WheelColliders[1].motorTorque = thrustTorque / 3;

        if (handbrakeTorque > 0)
        {
            m_WheelColliders[2].brakeTorque = Mathf.Max(m_WheelColliders[2].brakeTorque, handbrakeTorque);
            m_WheelColliders[3].brakeTorque = Mathf.Max(m_WheelColliders[3].brakeTorque, handbrakeTorque);
            m_WheelColliders[2].motorTorque = 0;
            m_WheelColliders[3].motorTorque = 0;
        }
        else
        {
            m_WheelColliders[2].motorTorque = thrustTorque;
            m_WheelColliders[3].motorTorque = thrustTorque;
        }

        if (accel <= 0)
        {
            foreach (var collider in m_WheelColliders)
            {
                collider.brakeTorque = Mathf.Max(collider.brakeTorque, m_BrakeTorque);
            }
        }
    }

    private void AddDownForce()
    {
        var body = m_WheelColliders[0].attachedRigidbody;

        body.AddForce(-transform.up * m_Downforce * body.velocity.magnitude); 
    }
}

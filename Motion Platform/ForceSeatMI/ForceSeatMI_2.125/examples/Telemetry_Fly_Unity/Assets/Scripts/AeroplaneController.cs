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

public class AeroplaneController : MonoBehaviour
{
    // Maximum engine power
    [SerializeField] private float m_MaxEnginePower = 40f;

    // Lift generated while moving forwards
    [SerializeField] private float m_Lift = 0.002f;

    // Speed at which lift generation stops
    [SerializeField] private float m_ZeroLiftSpeed = 300;

    // Roll effect amplification
    [SerializeField] private float m_RollEffect = 1f;

    // Pitch effect amplification
    [SerializeField] private float m_PitchEffect = 1f;

    // Yaw effect amplification
    [SerializeField] private float m_YawEffect = 0.2f;

    // Banked turn effect amplification
    [SerializeField] private float m_BankedTurnEffect = 0.5f;

    // Affects the speed of aeroplane
    [SerializeField] private float m_AerodynamicEffect = 0.02f;

    // Factor that determines how automaticall pitch is applied when in a banked turn
    [SerializeField] private float m_AutoTurnPitch = 0.5f;

    // Auto leveling factor when not rolling
    [SerializeField] private float m_AutoRollLevel = 0.2f;

    // Auto levelign factor when not pitching
    [SerializeField] private float m_AutoPitchLevel = 0.2f;

    // How air brakes effect the drag
    [SerializeField] private float m_AirBrakesEffect = 3f;

    // Chnage speed of the throttle
    [SerializeField] private float m_ThrottleChangeSpeed = 0.3f;

    // How to increase drag with speed
    [SerializeField] private float m_DragIncreaseFactor  = 0.001f;

    public float Altitude       { get; private set; }
    public float Throttle       { get; private set; }
    public bool  AirBrakes      { get; private set; }
    public float ForwardSpeed   { get; private set; }
    public float EnginePower    { get; private set; }
    public float MaxEnginePower { get { return m_MaxEnginePower; } }
    public float RollAngle      { get; private set; }
    public float PitchAngle     { get; private set; }
    public float RollInput      { get; private set; }
    public float PitchInput     { get; private set; }
    public float YawInput       { get; private set; }
    public float ThrottleInput  { get; private set; }

    private float m_OriginalDrag;
    private float m_OriginalAngularDrag;
    private float m_AeroFactor;
    private float m_BankedTurnAmount;

    // Aeroplane body object
    private Rigidbody m_Rigidbody;

    // ForceSeatMI API
    private ForceSeatMI_Unity m_Api;
    private ForceSeatMI_Aeroplane m_aeroplane;
    private ForceSeatMI_Unity.ExtraParameters m_extraParameters;

    private void Start()
    {
        m_Rigidbody = GetComponent<Rigidbody>();
        m_OriginalDrag = m_Rigidbody.drag;
        m_OriginalAngularDrag = m_Rigidbody.angularDrag;

        // ForceSeatMI - BEGIN
        m_Api = new ForceSeatMI_Unity();
        m_aeroplane = new ForceSeatMI_Aeroplane(m_Rigidbody);
        m_extraParameters = new ForceSeatMI_Unity.ExtraParameters();

        m_Api.SetAppID(""); // If you have dedicated app id, remove ActivateProfile calls from your code
        m_Api.ActivateProfile("SDK - Plane Telemetry ACE");
        m_Api.SetTelemetryObject(m_aeroplane);
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
        AirBrakes     = Input.GetButton("Fire1");
        RollInput     = Input.GetAxis("Horizontal");
        PitchInput    = Input.GetAxis("Vertical");
        YawInput      = 0;
        ThrottleInput = AirBrakes ? -1 : 1;

        // Move aeroplane using user's input
        Move();

        // ForceSeatMI - BEGIN

        // Use extra parameters to generate custom effects, for exmp. vibrations. They will NOT be
        // filtered, smoothed or processed in any way.
        m_extraParameters.yaw     = 0;
        m_extraParameters.pitch   = 0;
        m_extraParameters.roll    = (float)Math.Sin(Time.fixedTime * 1000) * 0.02f;
        m_extraParameters.right   = 0;
        m_extraParameters.up      = 0;
        m_extraParameters.forward = 0;

        m_Api.AddExtra(m_extraParameters);
        m_Api.Update(Time.fixedDeltaTime);
        // ForceSeatMI - END
    }

    public void Move()
    {
        ClampInputs();
        CalculateRollAndPitchAngles();
        AutoLevel();
        CalculateForwardSpeed();
        ControlThrottle();
        CalculateDrag();
        CaluclateAerodynamicEffect();
        CalculateLinearForces();
        CalculateTorque();
        CalculateAltitude();
    }

    private void ClampInputs()
    {
        RollInput     = Mathf.Clamp(RollInput, -1, 1);
        PitchInput    = Mathf.Clamp(PitchInput, -1, 1);
        YawInput      = Mathf.Clamp(YawInput, -1, 1);
        ThrottleInput = Mathf.Clamp(ThrottleInput, -1, 1);
    }

    private void CalculateRollAndPitchAngles()
    {
        var flatForward = transform.forward;
        flatForward.y = 0;

        if (flatForward.sqrMagnitude > 0)
        {
            flatForward.Normalize();

            var localFlatForward = transform.InverseTransformDirection(flatForward);
            var flatRight        = Vector3.Cross(Vector3.up, flatForward);
            var localFlatRight   = transform.InverseTransformDirection(flatRight);

            PitchAngle = Mathf.Atan2(localFlatForward.y, localFlatForward.z);
            RollAngle  = Mathf.Atan2(localFlatRight.y, localFlatRight.x);
        }
    }
    
    private void AutoLevel()
    {
        m_BankedTurnAmount = Mathf.Sin(RollAngle);

        if (RollInput == 0f)
        {
            RollInput = -RollAngle * m_AutoRollLevel;
        }

        if (PitchInput == 0f)
        {
            PitchInput  = -PitchAngle * m_AutoPitchLevel;
            PitchInput -= Mathf.Abs(m_BankedTurnAmount * m_BankedTurnAmount * m_AutoTurnPitch);
        }
    }

    private void CalculateForwardSpeed()
    {
        ForwardSpeed = Mathf.Max(0, transform.InverseTransformDirection(m_Rigidbody.velocity).z);
    }

    private void ControlThrottle()
    {
        Throttle    = Mathf.Clamp01(Throttle + ThrottleInput * Time.deltaTime * m_ThrottleChangeSpeed);
        EnginePower = Throttle * m_MaxEnginePower;
    }

    private void CalculateDrag()
    {
        float extraDrag = m_Rigidbody.velocity.magnitude * m_DragIncreaseFactor;

        m_Rigidbody.drag = (AirBrakes ? (m_OriginalDrag + extraDrag) * m_AirBrakesEffect : m_OriginalDrag + extraDrag);
        m_Rigidbody.angularDrag = m_OriginalAngularDrag * ForwardSpeed;
    }

    private void CaluclateAerodynamicEffect()
    {
        if (m_Rigidbody.velocity.magnitude > 0)
        {
            m_AeroFactor  = Vector3.Dot(transform.forward, m_Rigidbody.velocity.normalized);
            m_AeroFactor *= m_AeroFactor;

            var newVelocity = Vector3.Lerp(m_Rigidbody.velocity,
                transform.forward * ForwardSpeed,
                m_AeroFactor * ForwardSpeed * m_AerodynamicEffect * Time.deltaTime);

            m_Rigidbody.velocity = newVelocity;
            m_Rigidbody.rotation = Quaternion.Slerp(m_Rigidbody.rotation,
                                                    Quaternion.LookRotation(m_Rigidbody.velocity, transform.up),
                                                    m_AerodynamicEffect * Time.deltaTime);
        }
    }

    private void CalculateLinearForces()
    {
        var forces = Vector3.zero;
        forces += EnginePower * transform.forward;

        var liftDirection  = Vector3.Cross(m_Rigidbody.velocity, transform.right).normalized;
        var zeroLiftFactor = Mathf.InverseLerp(m_ZeroLiftSpeed, 0, ForwardSpeed);
        var liftPower      =  ForwardSpeed * ForwardSpeed * m_Lift * zeroLiftFactor * m_AeroFactor;

        forces += liftPower * liftDirection;

        m_Rigidbody.AddForce(forces);
    }
    
    private void CalculateTorque()
    {
        var torque = Vector3.zero;
        torque += PitchInput * m_PitchEffect * transform.right;
        torque += YawInput * m_YawEffect * transform.up;
        torque += -RollInput * m_RollEffect * transform.forward;
        torque += m_BankedTurnAmount * m_BankedTurnEffect * transform.up;

        m_Rigidbody.AddTorque(torque * ForwardSpeed * m_AeroFactor);
    }

    private void CalculateAltitude()
    {
        var ray = new Ray(transform.position - Vector3.up * 10, -Vector3.up);
        RaycastHit hit;
        Altitude = Physics.Raycast(ray, out hit) ? hit.distance + 10 : transform.position.y;
    }
}

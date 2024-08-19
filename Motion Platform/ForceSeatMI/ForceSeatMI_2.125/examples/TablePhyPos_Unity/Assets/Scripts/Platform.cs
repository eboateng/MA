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
using System.Runtime.InteropServices;
using UnityEngine;

public class Platform : MonoBehaviour {
    // Heave maximum value that is available in the game
    private const float DRAWING_HEAVE_MAX = 1.0f;

    // Heave change step
    private const float DRAWING_HEAVE_STEP = 0.05f;

    // Maximum value of pitch angle that is available in the game
    private const float DRAWING_PITCH_MAX = 16;

    // Pitch change step
    private const float DRAWING_PITCH_STEP = 1;

    // Maximum value of roll angle that is available in the game
    private const float DRAWING_ROLL_MAX = 16;

    // Pitch change step
    private const float DRAWING_ROLL_STEP = 1;

    // Shaft object
    private GameObject m_shaft = null;

    // Board object
    private GameObject m_board = null;

    // Origin position of the shaft
    private Vector3 m_originPosition;

    // Origin rotation of the board
    private Vector3 m_originRotation;

    // Current platform's heave in game
    private float m_heave = 0;

    // Current platform's pitch in game
    private float m_pitch = 0;

    // Current platform's roll in game
    private float m_roll = 0;

    // FSMI api
    private ForceSeatMI m_fsmi;

    // Position in physical coordinates that will be send to the platform
    private FSMI_TopTablePositionPhysical m_platformPosition = new FSMI_TopTablePositionPhysical();

    void Start ()
    {
        // Load ForceSeatMI library from ForceSeatPM installation directory
        m_fsmi = new ForceSeatMI();

        if (m_fsmi.IsLoaded())
        {
            // Find platform's components
            m_shaft = GameObject.Find("Shaft");
            m_board = GameObject.Find("Board");

            SaveOriginPosition();
            SaveOriginRotation();

            // Prepare data structure by clearing it and setting correct size
            m_platformPosition.mask = 0;
            m_platformPosition.structSize = (byte)Marshal.SizeOf(m_platformPosition);

            m_platformPosition.state = FSMI_State.NO_PAUSE;

            // Set fields that can be changed by demo application
            m_platformPosition.mask = FSMI_POS_BIT.STATE | FSMI_POS_BIT.POSITION;

            m_fsmi.SetAppID(""); // If you have dedicated app id, remove ActivateProfile calls from your code
            m_fsmi.ActivateProfile("SDK - Positioning");
            m_fsmi.BeginMotionControl();

            SendDataToPlatform();
        }
        else
        {
            Debug.LogError("ForceSeatMI library has not been found!Please install ForceSeatPM.");
        }
    }
	
	void Update ()
    {
        if (m_fsmi.IsLoaded())
        {
            // Set back origin position and then modify it
            m_shaft.transform.position = m_originPosition;
            m_shaft.transform.Translate(0, m_heave, 0);

            // Set back origin rotation and then modify it
            m_board.transform.eulerAngles = m_originRotation;
            m_board.transform.Rotate(m_pitch, 0, -m_roll);

            SendDataToPlatform();
        }
    }

    void FixedUpdate()
    {
        // Update values in order to received user's input
        UpdateValue(ref m_pitch, Input.GetAxis("Vertical"), DRAWING_PITCH_STEP, -DRAWING_PITCH_MAX, DRAWING_PITCH_MAX);
        UpdateValue(ref m_roll, Input.GetAxis("Horizontal"), DRAWING_ROLL_STEP, -DRAWING_ROLL_MAX, DRAWING_ROLL_MAX);
        UpdateValue(ref m_heave, Input.GetKey(KeyCode.Space) ? 1 : 0, DRAWING_HEAVE_STEP, 0, DRAWING_HEAVE_MAX);
    }
    
    void OnDestroy()
    {
        if (m_fsmi.IsLoaded())
        {
            m_fsmi.EndMotionControl();
            m_fsmi.Dispose();
        }
    }

    private void UpdateValue(ref float value, float input, float step, float min, float max)
    {
        if (0 < input)
        {
            value = Mathf.Clamp(value + step, min, max);
        }
        else if (0 > input)
        {
            value = Mathf.Clamp(value - step, min, max);
        }
        else if (value > 0)
        {
            value = Mathf.Clamp(value - step, 0, max);
        }
        else if (value < 0)
        {
            value = Mathf.Clamp(value + step, min, 0);
        }
    }

    private void SaveOriginPosition()
    {
        // Save origin position of the platform's shaft
        var x = m_shaft.transform.position.x;
        var y = m_shaft.transform.position.y;
        var z = m_shaft.transform.position.z;

        m_originPosition = new Vector3(x, y, z);
    }

    private void SaveOriginRotation()
    {
        // Save origin rotation of the platform's board
        var x = m_board.transform.eulerAngles.x;
        var y = m_board.transform.eulerAngles.y;
        var z = m_board.transform.eulerAngles.z;

        m_originRotation = new Vector3(x, y, z);
    }

    private void SendDataToPlatform()
    {
        // Convert parameters to logical units
        m_platformPosition.state = FSMI_State.NO_PAUSE;
        m_platformPosition.roll = Mathf.Deg2Rad * m_roll;
        m_platformPosition.pitch = -Mathf.Deg2Rad * m_pitch;
        m_platformPosition.heave = m_heave * 100;
        
        // Send data to platform
        m_fsmi.SendTopTablePosPhy(ref m_platformPosition);
    }
}

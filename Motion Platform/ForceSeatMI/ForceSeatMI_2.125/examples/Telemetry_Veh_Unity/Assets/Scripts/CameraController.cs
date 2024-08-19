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

using UnityEngine;

public class CameraController : MonoBehaviour {
    public Transform VehicleTransform;
    public float DistanceFromVehicle;
    public float HeightFromVehicle;
    public float RotationDamping;
    public float HeightDamping;

    void LateUpdate()
    {
        var angle = VehicleTransform.eulerAngles.y;
        var height = HeightFromVehicle + VehicleTransform.position.y;

        var cameraAngle = transform.eulerAngles.y;
        var cameraHeight = transform.position.y;

        cameraAngle = Mathf.LerpAngle(cameraAngle, angle, RotationDamping * Time.deltaTime);
        cameraHeight = Mathf.Lerp(cameraHeight, height, HeightDamping * Time.deltaTime);

        var currentRotation = Quaternion.Euler(0, cameraAngle, 0);
        transform.position = VehicleTransform.position;
        transform.position -= currentRotation * Vector3.forward * DistanceFromVehicle;

        var tmp = transform.position;
        tmp.y = cameraHeight;

        transform.position = tmp;
        transform.LookAt(VehicleTransform);
    }
}

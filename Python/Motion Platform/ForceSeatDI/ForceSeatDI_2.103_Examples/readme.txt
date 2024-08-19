==============================================================================
ForceSeatDI - Direct Motion Interface for motion platform
Copyright (C) 2012-2020 Motion Systems

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
=============================================================================

The latest version of documentation can be find at following address:
https://motionsystems.eu/ref/fsdi-related

Running on Linux:
-----------------

1. An application that uses ForceSeatDI with USB motion platform has to:
   a) have writing permission to device
   b) be able to detach HID device from the kernel.

   This is why it is recommended to RUN THE APPLICATION AS 'root'.

2. ForceSeatDI32.RSPi_3.so is armv7l 4.9.35.

3. ForceSeatDI64.LinuxPC.so is Ubuntu 16.04.3 LTS x64

4. Make sure to rename ForceSeatDI32.RSPi_3.so to ForceSeatDI32.so when Raspberry Pi 3 is used

5. Make sure to rename ForceSeatDI64.LinuxPC.so to ForceSeatDI64.so when Linux PC Desktop is used

6. Make sure to copy ForceSeatDI32.so and/or ForceSeatDI64.so to the same directory where your 
   executable is located. 

7. Before you start your application, make sure that all dependences are installed. You can use 
   following commands to verify dependences:
    ldd ./ForceSeatDI32.RSPi_3.so
    ldd ./ForceSeatDI64.LinuxPC.so

8. libusb might be missing in Ubuntu. You can install it using following command:
    sudo apt-get install libusb-0.1-4

9. For Python examples, Python 3.x is required (tested with Python 3.4.3)

Linux troubleshooting:
-----------------------

ForceSeatDI races with kernel HID driver. By default the library tries to detach
the device from the kernel (this is why 'root' is required), but in case it fails, 
it might be required to do it manually (as 'root') before application is executed:
   sudo rmmod usbhid
   sudo modprobe usbhid quirks=0x0483:0xA110:0x04

Alternatively you can try to disable usbhid from loading:
a) edit or create:
    /etc/udev/rules.d/99-disable-usb-hid.rules 
b) add there following line:
    SUBSYSTEMS=="usb", DRIVERS=="usbhid", ACTION=="add", ATTR{authorized}="0"
c) restart udev
    sudo udevadm control --reload-rules && 
    sudo udevadm trigger

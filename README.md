Read me file for the master thesis scripts developed by Emmanuel Boateng

Copyright: Emmanuel Mensah Boaetng

Requirements
------------
- Requires the motion systems platform manager software ForceSeatPM including drivers installed and setup for communication with the motion platform

- Requires Python 3.8 and the following libraries installed using pip
  - serial_asyncio for async serial communication with Artemis OpenLog (if used. Otherwise this module must be commented out in the main.py file)
  - scipy for loading motion input data from mat file
  - pandas for storing recorded and motion data to cdv file
  - numpy motion control/trajectory math

The motion systems library (ForceSeatMI directory) must be available at the project root (same directory as the main.py file)

Usage
-----
 1. To run the motion compensation algorithm, run scripts in Python/Motion Platform folder.
   - main.py runs the motion platform using a sinusoidal motion
   - data.py runs the motion platform using pre-existing EOLOS buoy data
   - save_sensor.py runs the motion platform and also saves the sensor data of the motion platform
2. The motion compensation algorithm is located in the Python/Motion compensation folder
   - ps30_IMU_trafo_multi.py contains the code that uses sensor data to adjust the rotary stages using the PS 30 controller.
     The INS sensor must be connected to the same PC runnning this code.
   - other PS30 commands are attached in the folder and can beb used to enhance the program
   - Old coding prototypes are provided for inspiration.
3. The INS Sensor data can be saved separately using the save_sensor.py code located in the Python/INS Sensor folder. it facilitates comparison of motion platform and
  INS sensor data
4. Various videos captured and used in the machine vision algortihm can be found in the Data folder
5. The machine vision scripts used for validation in MATLAB are in the named folder
  - Run the camera calibration to get the parameters for the camera, then run simple comparison to compare the values captured in the video for validation
  - To compare the motion platform and sensor data, run the script MotionPlatform_Data_v3.m.
  - 

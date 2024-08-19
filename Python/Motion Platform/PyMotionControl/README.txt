Python Motion Control for the SWE Motion Platform developed during the master these by Maayen Wigger

Copyright: Maayen Wigger 2021

Requirements
------------
- Requires the motion systems platform manager software ForceSeatPM including drivers installed and setup for communication with the motion platform

- Requires Python 3.8 and the following libraries installed using pip

- prettyprint for console output
- serial_asyncio for async serial communication with Artemis OpenLog (if used. Otherwise this module must be commented out in the main.py file)
- scipy for loading motion input data from mat file
- pandas for storing recorded and motion data to cdv file
- numpy motion control/trajectory math

The motion systems library (ForceSeatMI directory) must be available at the project root (same directory as the main.py file)

Usage
-----

1. Modify main.py file if data acquisition using the Artemis OpenLog is not recuired.
2. Adjust the config to your needs
3. Run: python.exe main.py



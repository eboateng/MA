
from datetime import datetime
import asyncio
from typing import Optional

from ForceSeatMI import *
from .trajectory import Trajectory


class Platform:
    def __init__(self, frequency: Optional[float] = None,
                 trajectory: Optional[Trajectory] = None,
                 interface: Optional[str] = 'physical'):
        if frequency is None:
            self.frequency = 10.0
        else:
            self.frequency = frequency
        self.trajectory = trajectory
        self.interface = interface
        self.profile = 'SDK - Positioning'

        self.fsmi = ForceSeatMI()
        self.fsmi.activate_profile(self.profile)
        self.fsmi.begin_motion_control()
        self.position = None
        self.info = None

        self.variables = [var[0] for var in FSMI_PlatformInfo._fields_ if var[0].startswith('fk')]

        self.motion_log = {var: list() for var in self.variables}
        self.motion_log['timestamp'] = list()

        self.initialize()

    def initialize(self):
        if self.interface == 'matrix':
            self.position = FSMI_TopTableMatrixPhysical()
            self.position.structSize = sizeof(FSMI_TopTableMatrixPhysical)
            self.position.mask = FSMI_TopTableMatrixPhysicalMask()
        else:
            self.position = FSMI_TopTablePositionPhysical()
            self.position.structSize = sizeof(FSMI_TopTablePositionPhysical)
            self.position.mask = FSMI_TopTableMatrixPhysicalMask()

        self.position.maxSpeed = 65535

        self.info = FSMI_PlatformInfo()
        self.info.structSize = sizeof(FSMI_PlatformInfo)

    async def start_motion(self, trajectory: Optional[Trajectory] = None):
        if trajectory is not None:
            self.trajectory = trajectory
        if self.trajectory is None and trajectory is None:
            raise ValueError('No trajectory set')

        self.trajectory.begin()

        while self.trajectory.is_running():
            # now = time.time()
            now = datetime.utcnow()
            if self.interface == 'matrix':
                matrix = self.trajectory.get_matrix(self.position, now.timestamp())
                self.fsmi.send_top_table_matrix_phy(matrix)
            else:
                self.position = self.trajectory.get_table_physical(self.position, now.timestamp())
                self.fsmi.send_top_table_pos_phy(self.position)

            self.log(now)

            await asyncio.sleep(1/self.frequency)

        return self.motion_log

    def log(self, timestamp):
        self.fsmi.get_platform_info_ex(self.info, c_uint32(sizeof(FSMI_PlatformInfo)), c_uint32(100))

        self.motion_log['timestamp'].append(timestamp)
        for var in self.variables:
            try:
                self.motion_log[var].append(float(getattr(self.info, var)))
            except ValueError:
                self.motion_log[var].append(1)

    def end_motion(self):
        self.fsmi.park(FSMI_ParkMode_Normal())
        self.fsmi.end_motion_control()
        self.fsmi.delete()

from abc import ABC
import math
from collections import namedtuple

from ForceSeatMI import FSMI_TopTableMatrixPhysical, FSMI_TopTablePositionPhysical, FSMI_FLOAT

from datetime import datetime
from typing import Union, Optional


class Trajectory(ABC):
    axes = ['pitch', 'roll', 'yaw', 'surge', 'sway', 'heave']

    def __init__(self, **kwargs):
        if 'duration' in kwargs.keys():
            self.duration = kwargs.pop('duration')
        else:
            self.duration = 60  # seconds

        if 'center' in kwargs.keys():
            self.center = kwargs.pop('center')
        else:
            self.center = {'x': 0.0, 'y': 0.0, 'z': 0.0}
        self.center_correction = 126
        self._start = datetime.utcnow()

        for var in self.axes:
            if var in kwargs.keys():
                axis = kwargs[var]
                setattr(self, var, Axis(**axis))
            else:
                setattr(self, var, Axis())

    def is_running(self):
        return (datetime.utcnow() - self._start).seconds < self.duration

    def begin(self) -> Optional[datetime]:
        self._start = datetime.utcnow()
        return self._start

    def get_physical(self, x: Union[datetime, float]) -> dict:
        raise NotImplementedError

    def get_table_physical(self, position: FSMI_TopTablePositionPhysical, x: Union[datetime, float]) -> FSMI_TopTablePositionPhysical:
        phy = self.get_physical(x)
        for key, value in phy.items():
            setattr(position, key, value)

        return position

    def get_matrix(self, matrix: FSMI_TopTableMatrixPhysical, x: Union[datetime, float]):
        phy = self.get_physical(x)
        matrix = self.to_matrix(matrix, phy['pitch'], phy['roll'], phy['yaw'], phy['surge'], phy['sway'], phy['heave'])
        matrix = self.center_of_rotation(matrix, self.center['x'], self.center['y'], self.center['z'] + self.center_correction)
        return matrix

    def center_of_rotation(self, matrix: FSMI_TopTableMatrixPhysical, x: float, y: float, z: float):
        """
        correct position matrix for center of rotation
        """
        matrix.transformation[0][3] += x - matrix.transformation[0][0] * x - matrix.transformation[0][1] * y - \
                                       matrix.transformation[0][2] * z
        matrix.transformation[1][3] += y - matrix.transformation[1][0] * x - matrix.transformation[1][1] * y - \
                                       matrix.transformation[1][2] * z
        matrix.transformation[2][3] += z - matrix.transformation[2][0] * x - matrix.transformation[2][1] * y - \
                                       matrix.transformation[2][2] * z
        return matrix

    def to_matrix(self, matrix: FSMI_TopTableMatrixPhysical, ax: FSMI_FLOAT, ay: FSMI_FLOAT, az: FSMI_FLOAT, dx: FSMI_FLOAT,
                  dy: FSMI_FLOAT, dz: FSMI_FLOAT):
        sinAX = math.sin(math.radians(ax))
        cosAX = math.cos(math.radians(ax))
        sinAY = math.sin(math.radians(ay))
        cosAY = math.cos(math.radians(ay))
        sinAZ = math.sin(math.radians(az))
        cosAZ = math.cos(math.radians(az))

        matrix.transformation[0][0] = cosAY * cosAZ
        matrix.transformation[0][1] = cosAZ * sinAX * sinAY - cosAX * sinAZ
        matrix.transformation[0][2] = cosAX * cosAZ * sinAY + sinAX * sinAZ
        matrix.transformation[0][3] = dx

        matrix.transformation[1][0] = cosAY * sinAZ
        matrix.transformation[1][1] = cosAX * cosAZ + sinAX * sinAY * sinAZ
        matrix.transformation[1][2] = -cosAZ * sinAX + cosAX * sinAY * sinAZ
        matrix.transformation[1][3] = dy

        matrix.transformation[2][0] = -sinAY
        matrix.transformation[2][1] = cosAY * sinAX
        matrix.transformation[2][2] = cosAX * cosAY
        matrix.transformation[2][3] = dz

        matrix.transformation[3][0] = 0
        matrix.transformation[3][1] = 0
        matrix.transformation[3][2] = 0
        matrix.transformation[3][3] = 1

        return matrix


parameters = ['amplitude', 'period', 'phase', 'increment', 'range']


class Axis(namedtuple('Axis',
                      ['amplitude', 'period', 'phase', 'increment', 'range', 'offset'],
                      defaults=[0.0, 0.0, 0.0, None, None, 0.0])):
    __slots__ = ()

    def __str__(self):
        _str = 'Axis: '
        for par in self._fields:
            _str += f'{par}={getattr(self, par)}'
        return _str

import math
from datetime import datetime
from typing import Union

from .base import Trajectory


class Constant(Trajectory):
    def __init__(self, **kwargs):
        super(Constant, self).__init__(**kwargs)
        self.excitation_count = 10 # seconds

    def get_physical(self, t: Union[datetime, float]):
        pos = dict()
        for var in self.axes:
            axis = getattr(self, var)
            _amplitude = math.radians(axis.amplitude)
            pos[var] = _amplitude

        return pos

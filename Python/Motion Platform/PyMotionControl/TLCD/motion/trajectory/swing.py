
from datetime import datetime
import math
from typing import Union

from .base import Trajectory


class Swing(Trajectory):
    def __init__(self, **kwargs):
        super(Swing, self).__init__(**kwargs)

    def get_physical(self, t: Union[datetime, float]) -> dict:
        pos = dict()
        for var in self.axes:
            axis = getattr(self, var)
            _amplitude = math.radians(axis.amplitude)
            if axis.period > 0.0:
                _frequency = 1.0 / axis.period
            else:
                _frequency = 0.0
            _phase = math.radians(axis.phase)
            pos[var] = _amplitude * math.sin(2 * math.pi * _frequency * t + _phase)

        return pos

from datetime import datetime
import math
from typing import Union

from .base import Trajectory


class Sweep(Trajectory):
    def __init__(self, **kwargs):
        super(Sweep, self).__init__(**kwargs)
        self.axis = 'roll'

    def get_physical(self, t: Union[datetime, float]) -> dict:
        pos = dict()
        for var in self.axes:
            axis = getattr(self, var)
            _amplitude = math.radians(axis.amplitude)
            if axis.increment is not None:
                _increment = axis.increment
            else:
                if axis.range is not None:
                    _increment = axis.range / self.duration
                else:
                    _increment = 0.0
            if axis.period > 0.0:
                _frequency = 1.0 / (axis.period + _increment*(t-self._start.timestamp()))
            else:
                _frequency = 0.0
            _phase = math.radians(axis.phase)
            pos[var] = _amplitude * math.sin(2.0 * math.pi * _frequency*(t-self._start.timestamp()) + _phase)
            if var == 'roll':
                print(_increment, 1/_frequency)

        return pos

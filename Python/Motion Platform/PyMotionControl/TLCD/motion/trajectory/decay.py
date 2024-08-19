import math
from datetime import datetime
from typing import Union

from .base import Trajectory


class Decay(Trajectory):
    def __init__(self, **kwargs):
        super(Decay, self).__init__(**kwargs)
        self.excitation_time = 20 # seconds
        self.mode = 'linear'

    def get_physical(self, t: Union[datetime, float]):
        pos = dict()
        for var in self.axes:
            axis = getattr(self, var)

            if axis.increment is not None:
                _decrement = axis.increment
            else:
                _decrement = 1 / self.duration
            _amplitude = math.radians(axis.amplitude*(1.0 - _decrement))
            _frequency = 1 / axis.period + axis.increment / 2 * t
            _phase = math.radians(axis.phase)
            pos[var] = _amplitude * math.sin(2 * math.pi * _frequency * t + _phase)

        return pos

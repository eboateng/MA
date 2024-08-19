import math
from datetime import datetime
from typing import Union

from .base import Trajectory


class Peak(Trajectory):
    def __init__(self, **kwargs):
        super(Peak, self).__init__(**kwargs)
        self.excitation_count = 10 # seconds
        self._state= 0
        self._last_state_time = 0

    def get_physical(self, t: Union[datetime, float]):
        pos = dict()
        for var in self.axes:
            axis = getattr(self, var)
            _amplitude = math.radians(axis.amplitude)
            _frequency = 1 / axis.period

            if divmod(t, _frequency/4)[0] < 1.0:
                pos[var] = _amplitude * math.sin(2*math.pi*_frequency*t)**2
            elif 1.0 <= divmod(t, _frequency/4)[0] <= (1.0 + 2*self.excitation_count):
                pos[var] = _amplitude
            elif (1.0 + 2*self.excitation_count) < divmod(t, _frequency/4)[0] < (2.0 + 2*self.excitation_count):
                pos[var] = _amplitude * math.sin(2 * math.pi * _frequency * t)**2
            else:
                pos[var] = 0.0

        return pos

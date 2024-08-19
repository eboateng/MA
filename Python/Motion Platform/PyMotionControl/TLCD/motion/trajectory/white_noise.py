
from datetime import datetime
import math
from typing import Union
import numpy as np
from numpy.random import default_rng

from .base import Trajectory

rng = default_rng()


class WhiteNoise(Trajectory):
    def __init__(self, **kwargs):
        super(WhiteNoise, self).__init__(**kwargs)
        self.N = 100
        self.frequencies = rng.uniform(0.15, 1.4, self.N)
        self.phases = np.linspace(0, 2*np.pi, self.N)
        self.norm = np.sum([np.sin(2 * np.pi * self.frequencies * x + self.phases) for x in np.linspace(0, 100, 1000)])

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
            _sig = np.sin(2 * np.pi * self.frequencies * t + self.phases)
            pos[var] = 1.5*_amplitude * np.sum(_sig)/self.norm
        return pos

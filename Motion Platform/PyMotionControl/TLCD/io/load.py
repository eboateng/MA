
from scipy.io import loadmat


def load_malibu(filepath):

    data = loadmat(filepath)
    pitch = [x for x in data['pitch']['Data'][0][0].flat]
    roll = [x for x in data['roll']['Data'][0][0].flat]
    return pitch, roll
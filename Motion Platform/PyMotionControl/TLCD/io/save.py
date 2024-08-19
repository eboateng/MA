
import pandas as pd
import config


def write_data(motion_data, liquid_data):
    """write gathered data to file on disk"""
    motion_df = pd.DataFrame(motion_data)
    motion_df.set_index('timestamp', inplace=True)
    water_df = pd.DataFrame(liquid_data)
    water_df.set_index('timestamp', inplace=True)
    date_str = water_df.index[0].strftime('%Y-%m-%d_%H-%M-%S')
    motion_df.to_csv(f'{config.data_output}/{date_str}_motion.csv')
    water_df.to_csv(f'{config.data_output}/{date_str}_liquid.csv')
    print('Data saved')

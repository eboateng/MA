
import asyncio
import importlib

import config
from TLCD.io.save import write_data
from TLCD.motion import Platform
from TLCD.artemis import ArtemisReceiver, ArtemisConsumer
from TLCD.motion.trajectory import Swing as Traj


async def main(port):
    # get the event loop
    loop = asyncio.get_event_loop()

    # create queue for measurement data exchange
    measurement_queue = asyncio.Queue()

    # initialize motion platform
    FS = Platform(config.frequency)
    print('Motion control initialized')

    # Serial connection receiver task initialized with queue
    ArtRec = ArtemisReceiver(measurement_queue, config.port, config.baudrate)
    receiver_task = asyncio.create_task(ArtRec.receive())
    print('Measurement data receiver started')

    # start measurement data consumer
    ArtCon = ArtemisConsumer(measurement_queue, config.artemis_columns)
    consumer_task = asyncio.create_task(ArtCon.consume())
    print('Measurement data consumer started')

    # initialize trajectory

    trajectory = Traj(duration=config.duration,
                      center=config.center,
                      pitch=config.pitch,
                      roll=config.roll,
                      sway=config.sway)

    # wait 2 seconds for the sensors to initialize before starting the motion
    # await asyncio.sleep(2)
    # run motion control
    motion_results = await FS.start_motion(trajectory=trajectory)
    print('Motion done. Waiting for measurement to finish...')
    await asyncio.sleep(2)

    # shutdown measurement data receiver
    receiver_task.cancel()
    ArtRec.writer.close()
    print("Measurement data receiver task stopped")

    # send stop message to measurement data consumer and wait
    # for the rest of data in que to be processed
    await measurement_queue.put(None)
    await asyncio.sleep(2)

    # collect results from consumer
    measurement_data = await asyncio.gather(*consumer_task)
    if not isinstance(measurement_data, dict):
        measurement_data = consumer_task.result()

    print("Consumer task finished")

    # Join queue
    await measurement_queue.join()
    print('Queue joined')
    # stop async loop
    loop.stop()

    print('End motion control')
    FS.end_motion()
    print('Done')
    write_data(motion_results, measurement_data)
dd

if __name__ == '__main__':
    # period_range = [1.5, 2.0, 2.5, 3.0, 3.5,
    #                 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9,
    #                 5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9,
    #                 6.0, 6.5, 7.0, 7.5, 8.0, 8.5,
    #                 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0]
    period_range = [4]
    for i in period_range:
        try:
            config.sway['period'] = i
            asyncio.run(main(config.port))
        except asyncio.CancelledError as e:
            raise(e)
        print(f'{i}s swing test done')

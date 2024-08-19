# import builtin
from datetime import datetime
import asyncio
from typing import AnyStr, SupportsInt, Optional

# import pypi
from serial_asyncio import open_serial_connection


class Receiver:
    def __init__(self, queue: asyncio.Queue, port: AnyStr, baudrate: Optional[SupportsInt]):
        self.queue = queue
        self.reader = None
        self.writer = None
        self.port = port
        if baudrate is None:
            self.baudrate = 112500
        else:
            self.baudrate = baudrate

    async def connect(self):
        try:
            self.reader, self.writer = await open_serial_connection(url=self.port,
                                                                    baudrate=self.baudrate)
            print(f'Connected to {self.port}')
        except(asyncio.TimeoutError, OSError) as e:
            print(f'Could not connect to {self.port}. Trying again ...')
            if isinstance(e, OSError):
                print(e)

    async def receive(self):
        await self.connect()
        while True:
            line = await self.reader.readline()
            now = datetime.utcnow()
            data = {'timestamp': now, 'data': line}
            self.queue.put_nowait(data)
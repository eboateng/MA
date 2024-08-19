# import builtin
import asyncio
from datetime import datetime
from typing import Optional, List, Any, AnyStr
from pprint import pprint


class Consumer:
    def __init__(self, queue: asyncio.Queue, header: Optional[List], debug:bool = False):
        self.queue = queue
        self.header = header
        self.debug = debug
        self.received = {'header': list(), 'timestamp': list(), 'data': list()}

    async def consume(self) -> dict:
        try:
            while True:
                item = await self.queue.get()
                # self.show(item)
                if item is None:
                    self.queue.task_done()
                    return self.received  # exit task
                else:
                    if b'rtcDate' in item['data']:
                        header = self.get_header(item, self.header)
                        self.received = {x: list() for x in header}
                    else:
                        _data = self.process(item['data'])
                        if _data is not None:
                            if len(_data) == (len(self.header)-1):
                                self.received['timestamp'].append(item['timestamp'])
                                for i, key in enumerate(self.header[1:]):
                                    self.received[key].append(_data[i])
                                self.show(item['data'])
                    self.queue.task_done()
        except asyncio.CancelledError:
            print('pass_received task cancelled')
        finally:
            return self.received

    def process(self, data_line: AnyStr) -> Optional[List]:
        """
        Processed data retrieved from queue
        data_line can
        b'01/01/2000,00:21:22.88,-5.86,-11.23,-1019.53,1.08,-2.24,-0.30,-13.65,-55.95,-64.65,29.47,22,0,95885.67,36.15,462.95,22.34,10.06,\r\n'

        Args:
            data_line: 

        Returns:
            list (optional): 
        """
        processed = list()
        data_list = data_line.decode('utf-8').strip().strip(',').split(',')
        try:
            timestamp = datetime.strptime(data_list[0] + ' ' + data_list[1], '%d/%m/%Y %H:%M:%S.%f')
            processed.append(timestamp)
            _data = [float(x) for x in data_list[2:]]
            processed.extend(_data)
        except (ValueError, IndexError) as e:
            print('Received measurement data corrupt')
            print(data_list)
            return None
        else:
            return processed

    def get_header(self, item, header):
        if header is None:
            header = item['data'].decode('utf-8').strip().strip(',').split(',')
            header[0] = 'timestamp'
        return header

    def show(self, item: Any) -> None:
        if self.debug:
            pprint(item)

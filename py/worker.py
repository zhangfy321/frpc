from conf.conf import DELIM
import server
from loguru import logger


class Worker:
    def __init__(self, inq, outq):
        self.inq = inq
        self.outq = outq
        self.get_cnt = 0
        self.miss_cnt = 0

    def _dispatcher(self):
        while True:
            logger
            try:
                fd, job = self.inq.popitem()  # todo collections.OrderedDict
                self.get_cnt += 1
            except:
                self.miss_cnt += 1
                continue
            for item in job.split(DELIM):
                if not len(item):
                    continue
                data = memoryview(item)
                method_id = int.from_bytes(data[:2], byteorder='big', signed=False)
                # logger.debug(f"method_id: {method_id}")
                if server.FUNCS.get(method_id):
                    self.outq[fd] = server.FUNCS[method_id](data[2:]) + DELIM
                else:
                    self.outq[fd] = bytearray(-1)

    def run(self):
        self._dispatcher()

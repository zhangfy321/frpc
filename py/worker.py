from conf.conf import DELIM
import server


class Worker:
    def __init__(self, inq, outq):
        self.inq = inq
        self.outq = outq

    def _dispatcher(self):
        while True:
            if not len(self.inq):
                continue
            fd, job = self.inq.popitem(False)  # todo collections.OrderedDict
            for item in job.split(DELIM):
                if not len(item):
                    continue
                data = memoryview(item)
                method_id = data[:2]
                if server.FUNCS.get(method_id):
                    self.outq[fd] = server.FUNCS[method_id](data[2:]) + DELIM
                else:
                    self.outq[fd] = bytearray(-1)

    def run(self):
        self._dispatcher()

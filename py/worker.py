import asyncio

class Worker:
    def __init__(self, inq, outq):
        self.inq = inq
        self.outq = outq

    async def _dispatcher(self):
        while True:
            if not len(self.inq): continue
            cur_job = self.inq.popitem(False) # todo collections.OrderedDict
            


# coding: utf8
import ioworker
import worker
import multiprocessing as mp
from conf.conf import *
from loguru import logger


# Server主要用来加载全局配置和启动多个worker
class Server:
    def __init__(self):
        mgr = mp.Manager()
        self.inq = mgr.dict()
        self.outq = mgr.dict()
        self.buffers = mgr.dict()

    def _set_config(self):
        pass

    def Run(self):
        IO_NUM = max(round(WORKER_NUM * 0.2), 1)
        ps = []
        for _ in range(IO_NUM):
            p = mp.Process(target=Server._run_ioloop, args=(self.inq, self.outq, self.buffers))
            p.daemon = True
            ps.append(p)
            
        for _ in range(WORKER_NUM - IO_NUM):
            p = mp.Process(target=Server._run_handler, args=(self.inq, self.outq))
            p.daemon = True
            ps.append(p)

        # 开始运行
        for p in ps:
            p.start()

        # 阻塞父进程
        for p in ps:
            p.join()

    @staticmethod
    def _run_ioloop(inq, outq, buffers):
        # logger.debug("_run_ioloop process running...")
        ioworker.IOWorker(inq, outq, buffers).run()

    @staticmethod
    def _run_handler(inq, outq):
        # logger.debug("_run_handler process running...")
        worker.Worker(inq, outq).run()

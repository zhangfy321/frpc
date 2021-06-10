# coding: utf8
import ioworker
import worker
# import multiprocessing as mp
from conf.conf import *
from loguru import logger
import threading as tr


# Server主要用来加载全局配置和启动多个worker
class Server:
    def __init__(self):
        # mgr = mp.Manager()
        # self.inq = mgr.dict()
        # self.outq = mgr.dict()
        # self.buffers = mgr.dict()
        self.inq = {}
        self.outq = {}
        self.buffers = {}

    def _set_config(self):
        raise NotImplementedError

    def Run(self):
        IO_NUM = min(round(WORKER_NUM * 0.8), WORKER_NUM - 1)
        ps = []
        logger.debug(f"_run_ioloop({IO_NUM}) process running...")
        lock = tr.Lock()
        for _ in range(IO_NUM):
            # p = mp.Process(target=Server._run_ioloop, args=(self.inq, self.outq, self.buffers))
            p = tr.Thread(target=Server._run_ioloop, args=(self.inq, self.outq, self.buffers, lock))
            p.daemon = True
            ps.append(p)

        logger.debug(f"_run_handler({WORKER_NUM - IO_NUM}) process running...")
        for _ in range(WORKER_NUM - IO_NUM):
            # p = mp.Process(target=Server._run_handler, args=(self.inq, self.outq))
            p = tr.Thread(target=Server._run_handler, args=(self.inq, self.outq))
            p.daemon = True
            ps.append(p)

        # 开始运行
        for p in ps:
            p.start()

        # # 阻塞父线程
        for p in ps:
            p.join()

    @staticmethod
    def _run_ioloop(inq, outq, buffers, lock):
        ioworker.IOWorker(inq, outq, buffers, lock).run()

    @staticmethod
    def _run_handler(inq, outq):
        worker.Worker(inq, outq).run()

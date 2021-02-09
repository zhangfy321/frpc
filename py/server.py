# coding: utf8
import ioworker
import worker


# Server主要用来加载全局配置和启动多个worker
class Server:
    def __init__(self):
        pass

    def _set_config(self):
        pass

    def Run(self):
        pass # 多进程多线程启动IO和Worker

    def _run_ioloop(self):
        ioloop = ioworker.IOWorker({}, {}, {})
        ioloop.run()

    def _run_handler(self):
        worker = worker.Worker()
        worker.run()

import select
import socket
from conf.conf import *
from multiprocessing import cpu_count


class Listener:
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)  # 能否解决惊群问题？
        self.timeout = TIMEOUT
        self.worker_num = WORKER_NUM if ENABLE_CUSTOMIZE_WORKER_NUM else cpu_count()  # 缺省为CPU物理核心数量
        self.socket.bind((ADDR, PORT))
        self.socket.listen(QUEUE_SIZE)
        self.socket.setblocking(False)  # todo
        self.listen_fd = self.socket.fileno()

    def run(self):
        raise NotImplementedError

    def _main_loop(self):
        raise NotImplementedError

    def _recv_data(self, raw_data):
        view = memoryview(raw_data).cast('B')  # unsigned char
        # 好处是按字节访问时减少拷贝，str和bytearray的切片操作会产生新的切片（拷贝数据）使用memoryview之后不会。
        while len(view) > 0:
            try:
                cnt = self.socket.recv_into(view)
                view = view[cnt:]
            except socket.error as err:
                pass

    def stop(self):
        # 监听信号 释放资源
        self.epoll.unregister(self.sock.fileno())
        self.epoll.close()
        self.sock.close()



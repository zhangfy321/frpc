from py.transport.listener import Listener
from multiprocessing import Lock
from os import fork
import select
from py.conf.conf import *


class MutexListener(Listener):
    def __init__(self):
        super().__init__()
        self.mutex = Lock()
        self.epoll = select.epoll()
        self.epoll.register(self.listen_fd, select.EPOLLIN)  # nginx在处理accept时采用LT
        self.child = []  # 保存子进程信息
        self.events = []

    def run(self):
        for _ in WORKER_NUM:
            pid = fork()
            if pid == 0:
                self._slave()  # 进入死循环, slave不会再执行fork()
            elif pid > 0:
                continue  # master进程继续fork()
            else:
                raise OSError

        self._master()  # 创建子进程完成后进入master逻辑

    def _master(self):
        while True:
            self.events = self.epoll.poll()
            if not len(self.events):
                continue
            for fd, e in self.events:
                if fd == self.listen_fd:  # 新的fd进来
                    conn, addr = self.socket.accept()
                    conn.setblocking(False)  # todo
                    self.epoll.register(conn.fileno(), select.EPOLLIN | select.EPOLLET)

                elif e == select.EPOLLET | select.EPOLLIN:
                    data = fd.recv(65536)
                    if data:
                        print(data)
                    self.epoll.modify(fd, select.EPOLLOUT)  # 读取完毕后设为可写事件
                elif e & select.EPOLLET | select.EPOLLIN:
                    self.socket.send("OK".encode())  # todo
                elif e & select.EPOLLHUP:
                    self.epoll.unregister(fd)
                    self.fd_map[fd].close()
                    del self.fd_map[fd]


                else:
                    continue

    def _slave(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, fileno=fd)

    def stop(self):
        pass

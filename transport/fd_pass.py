from transport.listener import Listener
from multiprocessing import Lock
from os import fork
import select


class PassFdListener(Listener):
    def __init__(self):
        super().__init__()
        self.mutex = Lock()
        self.epoll = select.epoll()
        # self.epoll.register(self.listen_fd, select.EPOLLIN | select.EPOLLET)
        self.epoll.register(self.listen_fd, select.EPOLLIN)  # nginx在处理accept时采用LT
        self.childs = [] # 保存子进程信息
        self.events = []

    def run(self):
        for _ in worker_num:
            pid = fork()
            if pid == 0:  # 子进程
                self._child()
            elif pid > 0:  # 父进程
                self._parent()
                pass  # todo wait()?
            else:
                raise OSError

    def _parent(self):
        while True:
            self.events = self.epoll.poll()
            if not len(self.events):
                continue
            for fd, e in self.events:
                if fd == self.listen_fd:
                    conn, addr = self.socket.accept()
                    self.mutex.release()
                    conn.setblocking(False)  # todo
                    self.epoll.register(conn.fileno(), select.EPOLLIN | select.EPOLLET)

                elif e & select.EPOLLHUP:
                    self.epoll.unregister(fd)
                    self.fd_map[fd].close()
                    del self.fd_map[fd]
                elif e & select.EPOLLIN:
                    data = fd.recv(65536)
                    if data:
                        print(data)
                    self.epoll.modify(fd, select.EPOLLOUT)  # 读取完毕后设为可写事件
                elif e & select.EPOLLOUT:
                    self.socket.send("OK".encode())  # todo
                else:
                    continue

    def _child(self):
        pass
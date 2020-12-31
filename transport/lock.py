from transport.listener import Listener
from multiprocessing import Lock
from os import fork
import select


class LockContentionListener(Listener):
    def __init__(self):
        super().__init__()
        self.mutex = Lock()

    def run(self):
        pid = fork()
        if pid == 0:  # 子进程
            self._main_loop()
        elif pid > 0:  # 父进程

        else:
            raise OSError


    def _main_loop(self):
        self.epoll = select.epoll()
        self.epoll.register(self.listen_fd, select.EPOLLIN | select.EPOLLET)
        while True:
            self.events = self.epoll.poll()
            if not len(self.events):
                continue
            for fd, e in self.events:
                if fd == self.listen_fd:
                    if not self.mutex.acquire(block=False): # 如果没有获取到锁，不再监听这个fd（从自己维护的epoll中删除）
                        self.epoll.register(fd, select.EPOLLIN | select.EPOLLET) # syscall todo 如果没找到呢
                    else:
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
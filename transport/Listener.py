import select
import socket

class Listener:
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_address = ("127.0.0.1", 8888)
        self.timeout = 10
        self.sock.bind(self.server_address)
        self.sock.listen(10)
        self.sock.setblocking(False)
        self.epoll = select.epoll()
        self.epoll.register(self.sock.fileno(), select.EPOLLIN)
        self.fd_map = {self.sock.fileno(): self.sock}

    def main_loop(self):
        while True:
            print("wait for connection")
            if not self.epoll.poll(self.timeout): continue
            for fd, e in events:
                cur_socket = self.fd_maps[fd]
                if self.fd_map[fd] == self.sock:
                    conn, addr = self.sock.accept()
                    conn.setblocking(False)
                    self.epoll.register(conn.fileno(), select.EPOLLIN)
                    self.fd_map[conn.fileno()] = conn
                elif e & select.EPOLLHUP:
                    self.epoll.unregister(fd)
                    self.fd_map[fd].close()
                    del self.fd_map[fd]
                elif event & select.EPOLLIN:
                    data = cur_socket.recv(65536)
                    if data: print(data)
                    epoll.modify(fd, select.EPOLLOUT)
                elif event & select.EPOLLOUT:
                    socket.socket.send("OK".encode()) #todo
                else:
                    pass


    def _recv_data(self, array):
        view = memoryview(array).cast('B') # 好处是按字节访问时减少拷贝，str和bytearray的切片操作会产生新的切片（拷贝数据）使用memoryview之后不会。
        while len(view) > 0:
            try:
                cnt = self.socket.recv_into(view)
                view = view[cnt:]
            except socket.error as err:
                self.logger.error('%s connection error %s ,will reconnect '%(self.ip, err))
                self.reconnect()


    def stop(self):
        # 监听信号 释放资源
        self.epoll.unregister(self.sock.fileno())
        self.epoll.close()
        self.sock.close()



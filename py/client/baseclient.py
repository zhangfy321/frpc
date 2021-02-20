import socket
from loguru import logger


class Client:
    def __init__(self, service, pool):
        self.service = service
        self.pool = pool

    def call(self, data):
        sock = self.pool.get()
        if not sock:
            logger.error("socket not enough")
            return None
        # todo
        sock.connect(('localhost', 8001))
        sock.send(data)
        data = sock.recv(100)
        print(f'Received: {data.decode()!r}')


class Pool:
    def __init__(self, size):
        self.pool = []
        for item in range(size):
            self.pool.append(socket.socket())

    def get(self):
        return self.pool[0] if self.pool else False

    def put(self, sock):
        self.pool.append(sock)

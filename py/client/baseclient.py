import socket
from loguru import logger
from conf.conf import *


class Client:
    def __init__(self):
        pass

    def call(self, method_id, req):
        sock = socket.socket()
        data = method_id.to_bytes(2, 'big') + req.SerializeToString() + DELIM
        sock.connect(('localhost', 8001))
        sock.send(data)
        data = sock.recv(100)
        sock.close()
        data = data.split(DELIM)
        if not len(data):
            return None
        return data[0]

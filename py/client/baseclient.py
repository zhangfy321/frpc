import socket
from loguru import logger
from conf.conf import *
from main import m_consul


class Client:
    def __init__(self):
        pass

    def call(self, service_name, method_id, req):
        sock = socket.socket()
        data = method_id.to_bytes(2, 'big') + req.SerializeToString() + DELIM
        _, addr = m_consul.get_service(service_name)
        logger.debug(f"[consul] get service {addr}")
        # sock.connect(("9.135.11.35", 8002))
        # sock.connect(addr)
        # sock.connect(("9.135.11.35", 8080))
        sock.connect(("192.168.10.7", 8001))


        sock.send(data)
        data = sock.recv(1024)
        sock.close()
        data = data.split(DELIM)
        if not len(data):
            return None
        return data[0]

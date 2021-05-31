import socket
from loguru import logger
from conf.conf import *

logger.add("log/web.log")


def init_server_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if sock == -1:
        logger.error("init_server_socket failed")
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT,
                    1)  # 内核级负载均衡 https://blog.csdn.net/dog250/article/details/51510823
    sock.setblocking(True)
    sock.bind((ADDR, PORT))
    sock.listen(QUEUE_SIZE)
    return sock

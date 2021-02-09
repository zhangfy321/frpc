from proto import helloworld_pb2
from loguru import logger
import select
import utils.base as base
from conf.conf import *
from queue import Queue
import socket


class IOWorker:
    def __init__(self):
        self.m_sock = base.init_server_socket()
        self.epoll = select.epoll()
        self.m_sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self.epoll.register(self.m_sock.fileno(), select.EPOLLIN | select.EPOLLET)
        self.conns = {}
        self.inq = {}
        self.outq = {}
        self.buffers = {}

    def run(self):
        while True:
            epoll_lst = self.epoll.poll()
            for fd, ev in epoll_lst:
                if fd == self.m_sock.fileno():
                    try:
                        while True:
                            conn, addr = self.m_sock.accept()
                            conn.setblocking(0)
                            logger.debug(f"new connection incomes: {addr}")
                            self.epoll.register(conn.fileno(), select.EPOLLIN | select.EPOLLET)
                            self.conns[conn.fileno()] = conn
                            self.inq[conn.fileno()] = Queue(QUEUE_MAXIMUM)
                            self.outq[conn.fileno()] = bytearray()
                            self.buffers[conn.fileno()] = bytearray()
                    except Exception as e:
                        logger.error(e)
                elif ev & select.EPOLLIN:
                    conn = self.conns[fd]
                    data = bytearray()
                    logger.debug(f"EPOLL IN: {fd}")
                    try:
                        while True:
                            data += conn.recv(1024)
                    except Exception as e:
                        logger.error(e)
                    logger.debug(f"recv data legth: {len(data)}")
                    msgs = data.split(DELI)  # 每个消息尾部加上DELI
                    if len(msgs) == 0:
                        self.buffers[fd] = self.buffers[fd] + bytearray(data)
                    else:
                        for idx, msg in enumerate(msgs):
                            if len(msg):
                                last = self.buffers.get(fd)
                                if idx < len(msgs):  # 后面还有说明本段构成了完整消息
                                    self.inq[fd].put(last + bytearray(msg))
                                    logger.debug("put to inq...")
                                    self.buffers[fd] = bytearray()
                                else:
                                    self.buffers[fd] = bytearray(msg)
                        while not self.inq[fd].empty():
                            logger.debug(f"message: {self.inq[fd].get()}")

                    logger.debug(f"in: {data}")
                    self.epoll.modify(fd, select.EPOLLOUT | select.EPOLLET)

                elif ev & select.EPOLLOUT:
                    data = memoryview(self.outq[fd])
                    logger.debug(f"out: {data}")
                    while len(data):
                        cnt = self.conns[fd].send(data)
                        data = data[cnt:]
                    if not len(data):
                        self.epoll.modify(fd, select.EPOLLET)
                        self.conns[fd].shutdown(socket.SHUT_RDWR)

                else:
                    logger.debug("CLOSE OR ERROR")
                    self.epoll.unregister(fd)
                    self.conns[fd].close()
                    del self.conns[fd]
                    del self.inq[fd]
                    del self.outq[fd]

    def decode(data):
        req = HelloRequest()
        req.ParseFromString(data)
        logger.debug(f"proto2string: {req.SerializeToString()}")

    def encode(data):
        pass

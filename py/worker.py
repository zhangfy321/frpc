from proto.helloworld_pb2 import HelloRequest
from proto import helloworld_pb2
from loguru import logger
import select
import utils.base as base
from conf.conf import *
from queue import Queue


class Worker:
    def __init__(self):
        self.m_sock = base.init_server_socket()
        self.epoll = select.epoll()
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
                    conn, addr = self.m_sock.accept()
                    logger.debug(f"new connection incomes: {addr}")
                    self.epoll.register(conn.fileno(), select.EPOLLIN | select.EPOLLET)
                    self.conns[conn.fileno()] = conn
                    self.inq[conn.fileno()] = Queue(QUEUE_MAXIMUM)
                    self.outq[conn.fileno()] = Queue(QUEUE_MAXIMUM)

                elif ev & select.EPOLLIN:
                    conn = self.conns[fd]
                    data = conn.recv(1024)
                    if len(data) < 0:
                        continue
                    msgs = data.split(DELI)  # 每个消息尾部加上DELI
                    if len(msgs) == 0:
                        self.buffers[fd] = self.buffers[fd] + bytearray(data)
                    else:
                        for idx, msg in enumerate(msgs):
                            if len(msg):
                                last = self.buffers[fd]
                                if idx < len(msgs):  # 后面还有说明本段构成了完整消息
                                    self.inq[fd].put(last + bytearray(msg))
                                    self.buffers[fd] = bytearray()
                                else:
                                    self.buffers[fd] = bytearray(msg)

                    logger.debug(f"in: {data}")
                    self.epoll.modify(fd, select.EPOLLOUT)

                elif ev & select.EPOLLOUT:
                    self.inq[fd].send(self.outq.get(fd))
                    logger.debug(f"out: {self.outq.get(fd)}")
                    self.epoll.modify(fd, select.EPOLLOUT)

                elif ev & select.EPOLLHUP:
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

from loguru import logger
import select
import utils.base as base
from conf.conf import *
from queue import Queue
import socket
import threading

class IOWorker: 
    def __init__(self, inq, outq, buffers, lock):
        logger.debug("init server socket")
        self.m_sock = base.init_server_socket()
        self.epoll = select.epoll()
        self.m_sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self.epoll.register(self.m_sock.fileno(), select.EPOLLIN | select.EPOLLET)
        self.conns = {} 
        self.inq = inq  # 入包
        self.outq = outq  # 出包
        self.buffers = buffers  # 用于处理粘包
        self.lock = lock

    def run(self):
        while True:
            epoll_lst = self.epoll.poll()
            for fd, ev in epoll_lst:
                logger.debug(f"e: {ev}, con {len(self.conns)}, 'inq/ouq' {len(self.inq)}/{len(self.outq)}")
                if fd == self.m_sock.fileno():
                    self.on_connect()
                elif ev & select.EPOLLOUT:
                    self.on_write(fd)
                elif ev & select.EPOLLIN:
                    self.on_read(fd)
                else:
                    self.on_close(fd)

    def on_connect(self):
        try:
            while True:
                conn, addr = self.m_sock.accept()
                conn.setblocking(0)
                self.epoll.register(conn.fileno(), select.EPOLLIN | select.EPOLLET)
                self.conns[conn.fileno()] = conn

                logger.debug(f"new connect {conn.getpeername()}")
                with self.lock:
                    self.inq[conn.fileno()] = bytearray()
                    self.outq[conn.fileno()] = bytearray()
                    self.buffers[conn.fileno()] = bytearray()
        except socket.error:
            pass

    def on_read(self, fd):
        logger.debug(f"new epoll in")
        conn = self.conns[fd]
        data = bytearray()
        try:
            while True:
                data += conn.recv(1024)
        except socket.error:
            pass
        logger.debug(f"read data {data}")
        msgs = data.split(DELIM)
        if len(msgs) == 0:  # 当前包不构成完整消息，与buffer拼接
            self.buffers[fd] = self.buffers[fd] + bytearray(data)
        else:
            for idx, msg in enumerate(msgs):
                if len(msg):
                    last = self.buffers.get(fd, bytearray())
                    if idx < len(msgs):  # 后面还有说明本段构成了完整消息
                        self.inq[fd] = last + bytearray(msg) + DELIM
                        self.buffers[fd] = bytearray()
                    else: 
                        self.buffers[fd] = bytearray(msg)
        self.epoll.modify(fd, select.EPOLLOUT | select.EPOLLET)
 
    def on_write(self, fd):
        data = memoryview(self.outq[fd])
        if not len(self.outq[fd]):
            logger.debug("出包为空")
            self.epoll.modify(fd, select.EPOLLOUT | select.EPOLLET)
            return
        try:
            while len(data) > 0:
                cnt = self.conns[fd].send(data)
                data = data[cnt:]
        except socket.error:
            pass
        # self.conns[fd].sendall(self.outq[fd])
        # self.epoll.modify(fd, select.EPOLLIN | select.EPOLLET)
        self.on_close(fd)

    def on_close(self, fd): # todo 超时控制 用堆？链表？map？
        self.conns[fd].shutdown(socket.SHUT_RDWR)
        self.epoll.unregister(fd)
        self.conns[fd].close()
        del self.conns[fd]
        with self.lock:
            try:
                del self.outq[fd]
                del self.buffers[fd]
            except:
                pass

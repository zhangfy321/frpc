from loguru import logger
import select
import utils.base as base
from conf.conf import *
from queue import Queue
import socket


class IOWorker: 
    def __init__(self, inq, outq, buffers):
        self.m_sock = base.init_server_socket()
        self.epoll = select.epoll()
        self.m_sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self.epoll.register(self.m_sock.fileno(), select.EPOLLIN | select.EPOLLET)

        self.conns = {} 
        self.inq = inq  # 入包
        self.outq = outq  # 出包
        self.buffers = buffers  # 用于处理粘包

    def run(self):
        while True:
            epoll_lst = self.epoll.poll()
            for fd, ev in epoll_lst:
                if fd == self.m_sock.fileno(): self.on_connect(fd)
                elif ev & select.EPOLLIN: self.on_read(fd)
                elif ev & select.EPOLLOUT: self.on_write(fd)
                else: self.on_close(fd)

    def on_connect(self, fd):
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

    def on_read(self, fd):
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
 
    def on_write(self, fd):
        logger.debug("EPOLL OUT")
        if not len(self.outq[fd]): # 没有回包，下次再说
            self.epoll.modify(fd, select.EPOLLOUT | select.EPOLLET) # 手动触发out事件
            return
        data = memoryview(self.outq[fd])
        logger.debug(f"out: {data}")
        try:
            while len(data):
                cnt = self.conns[fd].send(data)
                data = data[cnt:]
        except Exception as e:
            logger.error(e)
            if len(data): return  # 缓冲不够 没有写完 等下次OUT事件再写
        if not len(data):
            self.epoll.modify(fd, select.EPOLLET)
            self.conns[fd].shutdown(socket.SHUT_RDWR)  # 如何处理对端关闭？ 

    def on_close(self, fd):
        logger.debug("CLOSE OR ERROR")   # todo 超时控制 用堆？链表？map？
        self.epoll.unregister(fd)
        self.conns[fd].close()
        del self.conns[fd]
        del self.inq[fd]
        del self.outq[fd]

# class Sock: # 计时/排序
#     def __init__(self, fd):
#         self.fd = fd
#         self.time = time.time()
#     def __lt__(self, other):
#         return self.time < other.time

from loguru import logger
from baseserver import Server
from discovery import _consul as consul
from conf.conf import *
import socket
logger.add("log/web.log", enqueue=True)  # 异步日志


if __name__ == '__main__':
    c = consul.Consul(CONSUL_HOST, CONSUL_PORT)
    c.register_service(SERVICE_NAME, socket.gethostbyname(socket.gethostname()), PORT)  # 服务注册
    s = Server()
    s.Run()


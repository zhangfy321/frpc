from loguru import logger
from baseserver import Server
from discovery import _consul as consul
from conf.conf import *
import socket
logger.add("log/web.log", enqueue=True)  # 异步日志
m_consul = consul.Consul(CONSUL_HOST, CONSUL_PORT)  # 服务发现

if __name__ == '__main__':
    m_consul.register_service(SERVICE_NAME, socket.gethostbyname(socket.gethostname()), PORT)  # 服务注册
    s = Server()
    s.Run()


import consul
from conf.conf import *
import socket
import time
from loguru import logger
'''
docker pull consul

开启一个集群，要求有3个server，同时开启管理ui
docker run -d --name=consul1 -p 8900:8500 -e CONSUL_BIND_INTERFACE=eth0 consul agent --server=true --bootstrap-expect=3 --client=0.0.0.0 -ui

docker run -d --name=consul2 -e CONSUL_BIND_INTERFACE=eth0 consul agent --server=true --client=0.0.0.0 --join 192.168.10.2

docker run -d --name=consul3 -e CONSUL_BIND_INTERFACE=eth0 consul agent --server=true  --client=0.0.0.0 --join 192.168.10.2

docker run -d --name=consul4 -e CONSUL_BIND_INTERFACE=eth0 consul agent --server=false --client=0.0.0.0 --join 192.168.10.2
'''


class Consul(object):
    def __init__(self, host, port):
        self._consul = consul.Consul(host, port)

    def register_service(self, service_name, host, port, tags=None):
        tags = tags or []
        # 注册服务
        self._consul.agent.service.register(
            service_name,
            service_name + '_' + time.strftime("%Y-%m-%d~%H:%M:%S", time.localtime()),  # service_id
            host,
            port,
            tags,
            # 健康检查ip端口，检查时间：5,超时时间：30，注销时间：30s
            check=consul.Check().tcp(host, port, "5s", "30s", "30s"))
        logger.debug(f"[consul] register service {service_name}@{host}:{port}")

    def deregister(self, service_name):
        de_result = self._consul.agent.service.deregister(service_name)
        check_result = self._consul.agent.check.deregister(service_name)
        return de_result, check_result

    def get_service(self, service_name):
        services = self._consul.agent.services()
        service = services.get(service_name)
        if not service:
            return None, None
        addr = (service['Address'], int(service['Port']))
        return service, addr


if __name__ == '__main__':
    consul_client = Consul(CONSUL_HOST, CONSUL_PORT)
    consul_client.register_service(SERVICE_NAME, socket.gethostbyname(socket.gethostname()), PORT)
    print(consul_client.get_service("maple"))

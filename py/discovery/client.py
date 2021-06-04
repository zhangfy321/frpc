from kazoo.client import KazooClient
import socket
from conf.conf import MODULE_NAME, PORT

zk = KazooClient(hosts='127.0.0.1:8082')
zk.start()


def unregister():
    pass


def register():
    nodename = f'/{MODULE_NAME}'
    zk.create(nodename, f'{socket.gethostbyname(socket.gethostname())}_{PORT}', ephemeral=True, sequence=True, makepath=True)
    # makepath 父节点不存在则创建
    # ephemeral 临时节点，挂了自动删除
    # 在创建节点名后面增加10位数字（创建一个 testplatform/test 节点，实际创建 testplatform/test0000000003，这串数字顺序递增）


if __name__ == '__main__':
    register()
    node = zk.get_children(f'/{MODULE_NAME}')
    print(node)

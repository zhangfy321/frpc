from kazoo.client import KazooClient
import time


zk = KazooClient(hosts='127.0.0.1:2181')
zk.start()


def test_ensure():
    zk.ensure_path('/my/favorite')


def test_create():
    nodename = '/kwsy/service'
    index = 1
    while index < 10:
        zk.create(nodename,'123.56.190.151_{index}'.format(index=index),ephemeral=True,sequence=True)
        time.sleep(10)
        index += 1


if __name__ == '__main__':
    #zk.delete('/kwsy')
    test_create()
    zk.stop()
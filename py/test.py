from proto.helloworld_pb2 import HelloRequest
from conf.conf import DELIM
import socket


def tcp_echo_client():
    sock = socket.socket()
    sock.connect(('localhost', 8001))
    req = HelloRequest()
    req.message = "hello"
    method_id = 1
    data1 = method_id.to_bytes(2, 'big') + req.SerializeToString() + DELIM
    print("send...")
    sock.send(data1)
    data = sock.recv(100)
    print(f'Received: {data.decode()!r}')
    sock.close()


if __name__ == '__main__':
    tcp_echo_client()

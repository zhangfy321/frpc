import socket

class Client:
    def __init__(self):
        self.client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.server_address = ('127.0.0.1', 8888)
        self.client_socket.connect(server_address)

    def set_config(self, **kwargs):
        raise NotImplementedError

    def test_call(self):
        data = "hello"
        self.client_socket.sendall(data) #todo
        print(self.client_socket.recv(65536))
        self.client_socket.close()
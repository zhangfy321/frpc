import asyncio
from proto.helloworld_pb2 import HelloRequest, HelloReply


async def tcp_echo_client(message):
    reader, writer = await asyncio.open_connection(
        '127.0.0.1', 8888)

    req = HelloRequest()
    req.name = "zhangfengyu"
    data = req.SerializeToString()

    length = len(data)
    writer.write(b'\r\n' + data + b'\r\n' + data)

    data = await reader.read(100)
    proto_lst = data.split(b'\r\n')
    print(len(proto_lst))
    print("0", proto_lst[0])
    req.ParseFromString(proto_lst[1])
    print(f'Received: {req.name}')
    req.ParseFromString(proto_lst[2])
    print(f'Received: {req.name}')

    print('Close the connection')
    writer.close()

if __name__ == '__main__':
    asyncio.run(tcp_echo_client('Hello World!'))

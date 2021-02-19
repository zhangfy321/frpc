import asyncio
from proto.helloworld_pb2 import HelloRequest
from conf.conf import DELI


async def tcp_echo_client():
    reader, writer = await asyncio.open_connection(
        '42.193.182.50', 8001)

    req = HelloRequest()
    req.message = "aeonzhang"
    data1 = req.SerializeToString() + DELI + req.SerializeToString()[:10]
    data2 = req.SerializeToString()[10:] + DELI
    writer.write(data1)
    writer.write(data2)
    data = await reader.read(100)
    print(f'Received: {data.decode()!r}')
    writer.close()


if __name__ == '__main__':
    asyncio.run(tcp_echo_client())

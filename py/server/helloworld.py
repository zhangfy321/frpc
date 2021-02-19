from proto.helloworld_pb2 import HelloRequest, HelloReply
from loguru import logger


def hello(message):
    req = HelloRequest()
    req.ParseFromString(message)
    logger.debug(req)
    rsp = HelloReply
    rsp.message = "world!"
    return rsp.SerializeToString()

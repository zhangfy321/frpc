from proto.pay_pb2 import *
from loguru import logger


def pay(message):
    req = PayReq()
    req.ParseFromString(message)
    logger.debug(req)

    rsp = PayReply()
    rsp.status = "支付成功"
    return rsp.SerializeToString()

from proto.order_pb2 import *
from loguru import logger


def order(message):
    req = OrderReq()
    req.ParseFromString(message)
    logger.debug(req)

    rsp = OrderReply()
    rsp.status = "生成订单成功"
    rsp.order_id = "780918209"
    return rsp.SerializeToString()

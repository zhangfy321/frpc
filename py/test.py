from proto.order_pb2 import *
from proto.helloworld_pb2 import *
from proto.getlist_pb2 import *
from proto.pay_pb2 import *
from client.baseclient import Client
from loguru import logger
import threading
import time


def test_getlist():
    method_id = 2
    req = GetListReq()
    req.category = "数码"
    rsp = GetListReply()
    rsp.ParseFromString(c.call(method_id, req))
    logger.debug(rsp)


def test_order():
    method_id = 3
    req = OrderReq()
    rsp = OrderReply()
    req.good_id = "1"
    req.user_id = "1"
    rsp.ParseFromString(c.call(method_id, req))
    logger.debug(rsp)


def test_pay():
    method_id = 4
    req = PayReq()
    rsp = PayReply()
    req.order_id = "123124123"
    req.pay_code = "12121432324234"

    rsp.ParseFromString(c.call(method_id, req))
    logger.debug(rsp)


def test_hello():
    method_id = 1
    req = HelloRequest()
    rsp = HelloReply()
    req.message = "Hello"
    rsp.ParseFromString(c.call(method_id, req))
    logger.debug(rsp)


if __name__ == '__main__':
    c = Client()
    test_hello()
    # test_getlist()
    # test_order()
    # test_pay()
    ts = []
    for _ in range(100):
        t = threading.Thread(target=test_hello)
        ts.append(t)
    a = time.time()
    for t in ts:
        t.start()

    print(time.time() - a)

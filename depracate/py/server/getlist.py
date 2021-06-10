from proto.getlist_pb2 import *
from loguru import logger


items = {
    '数码': [
        {
            "id": "1",
            "name": "iPhone12",
            "price": "3321"
        },
        {
            "id": "2",
            "name": "iPad Air 4",
            "price": "13321"
        },
        {
            "id": "3",
            "name": "MacBook Pro 15",
            "price": "23321"
        }
    ]
}


def getlist(message):
    req = GetListReq()
    req.ParseFromString(message)
    logger.debug(req)

    rsp = GetListReply()
    if req.category == "数码":
        for item in items[req.category]:
            cur = rsp.list.add()
            cur.id = item["id"]
            cur.name = item['name']
            cur.price = item['price']

    return rsp.SerializeToString()

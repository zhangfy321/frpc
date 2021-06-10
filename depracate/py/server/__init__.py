import server.helloworld
import server.getlist
import server.pay
import server.order
# 此处定义可供外部调用的函数
FUNCS = {
    1: helloworld.hello,
    2: getlist.getlist,
    3: order.order,
    4: pay.pay
}
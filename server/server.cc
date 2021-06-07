#include "server.h"
#include "comm/format.h"
#include "io/io_loop.h"
namespace frpc
{

int Server::Run(){
    DEBUG("frpc server start running...");
    frpc::IOLoop io_loop = frpc::IOLoop(ip_, port_);
    io_loop.Run();
    return 0;
}

int Server::BeforeMasterRun(){
    return 0;
}

int Server::AfterMasterRun(){
    return 0;
}

int Server::BeforeChildRun(){
    return 0;
}

int Server::AfterChildRun(){
    return 0;
}
}
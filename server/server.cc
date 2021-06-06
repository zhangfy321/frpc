#include "server.h"
#include "comm/format.h"

namespace frpc
{

int Server::Run(){
    DEBUG("frpc server start running...");
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
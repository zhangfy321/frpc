#include <iostream>
#include "server/server.h"

int main() {
    std::cout << "hello world" << std::endl;
    frpc::Server* svr = new frpc::Server(1, "0.0.0.0", 8000);
    svr->Run();
}


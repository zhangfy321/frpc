#include <iostream>
#include "server/server.h"

int main() {
    frpc::Server* svr = new frpc::Server(1, "0.0.0.0", 8765);
    svr->Run();
}


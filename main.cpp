#include <iostream>
#include "server/server.h"
#include "demo/client.h"
int main() {
    frpc::Server* svr = new frpc::Server(1, "0.0.0.0", 8765);
    svr->Run();
    // SendToOneShot("hello", "127.0.0.1", 8765);
}


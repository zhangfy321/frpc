#include <stdlib.h>
#include "demo/client.h"
#include "comm/format.h"
#include "comm/macros.h"
#include <jemalloc/jemalloc.h>


void SendToOneShot(const std::string data, const std::string ip, uint16_t port){
    int fd = InitSocket(ip, port);
    // while (1) {
    {
        char* buf = static_cast<char*>(malloc(1 * sizeof(char)));
        
        send(fd, data.c_str(), strlen(data.c_str()), 0);
        DEBUG("send %s to %s:%u", data.c_str(), ip.c_str(), port);
        size_t len = recv(fd, buf, sizeof(buf), 0);
        DEBUG("recv len %lu", len);
    }

    close(fd);
}

int InitSocket(const std::string ip, uint16_t port){
    int client = socket(AF_INET, SOCK_STREAM, 0);
    RETURN_ERROR(client == -1, "create socket err");
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    int fd = connect(client, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr));
    RETURN_ERROR( fd < 0, "connect err");

    return fd;
}

int main() {
    SendToOneShot("hello", "127.0.0.1", 8765);
}
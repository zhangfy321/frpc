#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

void SendToOneShot(const std::string data, const std::string ip, uint16_t port);
int InitSocket(const std::string ip, uint16_t port);
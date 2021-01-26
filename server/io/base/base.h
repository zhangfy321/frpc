//
// Created by fy on 2021/1/15.
//

#ifndef FRPC_BASE_H
#define FRPC_BASE_H
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>

inline void LOG(const std::string &str)
{
    std::cout << str << std::endl;
}

int init_server_socket(const uint16_t *port);
bool connect(int fd, const std::string &ip, uint16_t port);
bool SendWithNoCopy(int fd, const char *buf, size_t len);
bool SendFileWithNoCopy(int out_fd, int in_fd, off_t *offset, size_t count);
bool Send(int fd, const void * buf, size_t len, int flags);
bool Close(int fd);
#endif //FRPC_BASE_H
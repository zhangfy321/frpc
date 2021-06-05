#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

#include <cassert>
#include <iostream>

namespace frpc 
{


Socket::Socket(const Socket& other) {
  fd_ = other.fd_;
  ip_ = other.ip_;
  port_ = other.port_;
}

Socket& Socket::operator=(const Socket& other) {
  fd_ = other.fd_;
  ip_ = other.ip_;
  port_ = other.port_;
  return *this;
}

Socket::~Socket() {}


void Socket::SetReuseAddr() {
    int flag = 1;
    if (-1 == setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, flag, sizeof(flag)))
    {
        ERROR("SetReuseAddr fail ret -1 flag %d", flag);
        exit(-1);
    }
}


void Socket::SetReusePort() {
#if defined(SO_REUSEPORT) && defined(TRPC_ENABLE_REUSEPORT)
  int flag = 1;
  if (SetSockOpt(SO_REUSEPORT, static_cast<const void*>(&flag),
                 static_cast<socklen_t>(sizeof(flag)), SOL_SOCKET) == -1) {
    assert(false);
  }
#endif
}

void Socket::Bind(const NetworkAddress& bind_addr) {
  assert(domain_ == bind_addr.Family());
  int ret = ::bind(fd_, bind_addr.Sockaddr(), bind_addr.Socklen());
  if (ret != 0) {
    TRPC_FMT_ERROR("Bind address {} error: {}", bind_addr.ToString(), strerror(errno));
    TRPC_ASSERT(false);
  }
}

void Socket::Bind(const UnixAddress& bind_addr) {
  if (::access(bind_addr.Path(), 0) != -1) {
    ::remove(bind_addr.Path());
  }
  int ret = ::bind(fd_, bind_addr.Sockaddr(), bind_addr.Socklen());
  if (ret != 0) {
    TRPC_FMT_ERROR("Bind address {} error: {}", bind_addr.Path(), strerror(errno));
    TRPC_ASSERT(false);
  }
}

void Socket::Close() {
  if (fd_ != -1) {
    ::close(fd_);
    fd_ = -1;
  }
}

void Socket::Listen(int backlog) {
  if (::listen(fd_, backlog) < 0) {
    assert(false);
  }
}

int Socket::Connect(const NetworkAddress& addr) {
  if (::connect(fd_, addr.Sockaddr(), addr.Socklen()) < 0 && errno != EINPROGRESS) {
    return -1;
  }
  return 0;
}

int Socket::Connect(const UnixAddress& addr) {
  if (::connect(fd_, addr.Sockaddr(), addr.Socklen()) < 0 && errno != EINPROGRESS) {
    return -1;
  }
  return 0;
}

int Socket::Recv(void* buff, size_t len, int flag) { return ::recv(fd_, buff, len, flag); }

int Socket::Send(const void* buff, size_t len, int flag) { return ::send(fd_, buff, len, flag); }

int Socket::RecvFrom(void* buff, size_t len, int flag, NetworkAddress* peer_addr) {
  struct sockaddr addr;
  socklen_t sock_len = static_cast<socklen_t>(sizeof(addr));
  int ret = ::recvfrom(fd_, buff, len, flag, &addr, &sock_len);
  if (ret > 0 && peer_addr != nullptr) {
    *peer_addr = NetworkAddress(&addr);
  }
  return ret;
}

int Socket::SendTo(const void* buff, size_t len, int flag, const NetworkAddress& peer_addr) {
  return ::sendto(fd_, buff, len, flag, peer_addr.Sockaddr(), peer_addr.Socklen());
}

int Socket::Writev(const struct iovec* iov, int iovcnt) { return ::writev(fd_, iov, iovcnt); }

int Socket::SendMsg(const struct msghdr* msg, int flag) { return ::sendmsg(fd_, msg, flag); }

void Socket::SetBlock(bool block) {
  assert(fd_ != -1);

  int val = 0;

  if ((val = ::fcntl(fd_, F_GETFL, 0)) == -1) {
    assert(false);
  }

  if (!block) {
    val |= O_NONBLOCK;
  } else {
    val &= ~O_NONBLOCK;
  }

  if (::fcntl(fd_, F_SETFL, val) == -1) {
    assert(false);
  }
}

int Socket::SetSockOpt(int opt, const void* val, socklen_t opt_len, int level) {
  return ::(fd_, level, opt, val, opt_len);
}

int Socket::GetSockOpt(int opt, void* val, socklen_t* opt_len, int level) {
  return ::getsockopt(fd_, level, opt, val, opt_len);
}

void Socket::SetNoCloseWait() {
  struct linger ling;
  ling.l_onoff = 1;  // 在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
  ling.l_linger = 0;  // 容许逗留的时间为0秒

  if (SetSockOpt(SO_LINGER, static_cast<const void*>(&ling), static_cast<socklen_t>(sizeof(linger)),
                 SOL_SOCKET) == -1) {
    assert(false);
  }
}

void Socket::SetCloseWait(int delay) {
  struct linger ling;
  ling.l_onoff = 1;  // 在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
  ling.l_linger = delay;  // 容许逗留的时间为delay秒

  if (SetSockOpt(SO_LINGER, static_cast<const void*>(&ling), static_cast<socklen_t>(sizeof(linger)),
                 SOL_SOCKET) == -1) {
    assert(false);
  }
}

void Socket::SetCloseWaitDefault() {
  struct linger ling;
  ling.l_onoff = 0;
  ling.l_linger = 0;

  if (SetSockOpt(SO_LINGER, static_cast<const void*>(&ling), static_cast<socklen_t>(sizeof(linger)),
                 SOL_SOCKET) == -1) {
    assert(false);
  }
}

void Socket::SetTcpNoDelay() {
  int flag = 1;

  if (SetSockOpt(TCP_NODELAY, static_cast<const void*>(&flag), static_cast<socklen_t>(sizeof(flag)),
                 IPPROTO_TCP) == -1) {
    assert(false);
  }
}

void Socket::SetKeepAlive() {
  int flag = 1;
  if (SetSockOpt(SO_KEEPALIVE, static_cast<const void*>(&flag),
                 static_cast<socklen_t>(sizeof(flag)), SOL_SOCKET) == -1) {
    assert(false);
  }
}

void Socket::SetSendBufferSize(int sz) {
  int flag = 1;
  if (SetSockOpt(SO_SNDBUF, static_cast<const void*>(&sz), static_cast<socklen_t>(sizeof(flag)),
                 SOL_SOCKET) == -1) {
    assert(false);
  }
}

int Socket::GetSendBufferSize() {
  int sz = 0;
  socklen_t len = sizeof(sz);
  if (GetSockOpt(SO_SNDBUF, static_cast<void*>(&sz), &len, SOL_SOCKET) == -1 || len != sizeof(sz)) {
    assert(false);
  }

  return sz;
}

void Socket::SetRecvBufferSize(int sz) {
  if (SetSockOpt(SO_RCVBUF, static_cast<const void*>(&sz), static_cast<socklen_t>(sizeof(sz)),
                 SOL_SOCKET) == -1) {
    assert(false);
  }
}

int Socket::GetRecvBufferSize() {
  int sz = 0;
  socklen_t len = sizeof(sz);
  if (GetSockOpt(SO_RCVBUF, static_cast<void*>(&sz), &len, SOL_SOCKET) == -1 || len != sizeof(sz)) {
    assert(false);
  }

  return sz;
}

}  // namespace trpc

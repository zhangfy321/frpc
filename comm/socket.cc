

// int Socket::Connect(const NetworkAddress& addr) {
//   if (::connect(fd_, addr.Sockaddr(), addr.Socklen()) < 0 && errno != EINPROGRESS) {
//     return -1;
//   }
//   return 0;
// }

// int Socket::Connect(const UnixAddress& addr) {
//   if (::connect(fd_, addr.Sockaddr(), addr.Socklen()) < 0 && errno != EINPROGRESS) {
//     return -1;
//   }
//   return 0;
// }

// int Socket::Recv(void* buff, size_t len, int flag) { return ::recv(fd_, buff, len, flag); }

// int Socket::Send(const void* buff, size_t len, int flag) { return ::send(fd_, buff, len, flag); }

// int Socket::RecvFrom(void* buff, size_t len, int flag, NetworkAddress* peer_addr) {
//   struct sockaddr addr;
//   socklen_t sock_len = static_cast<socklen_t>(sizeof(addr));
//   int ret = ::recvfrom(fd_, buff, len, flag, &addr, &sock_len);
//   if (ret > 0 && peer_addr != nullptr) {
//     *peer_addr = NetworkAddress(&addr);
//   }
//   return ret;
// }

// int Socket::SendTo(const void* buff, size_t len, int flag, const NetworkAddress& peer_addr) {
//   return ::sendto(fd_, buff, len, flag, peer_addr.Sockaddr(), peer_addr.Socklen());
// }

// int Socket::Writev(const struct iovec* iov, int iovcnt) { return ::writev(fd_, iov, iovcnt); }

// int Socket::SendMsg(const struct msghdr* msg, int flag) { return ::sendmsg(fd_, msg, flag); }

// void Socket::SetBlock(bool block) {
//   assert(fd_ != -1);

//   int val = 0;

//   if ((val = ::fcntl(fd_, F_GETFL, 0)) == -1) {
//     assert(false);
//   }

//   if (!block) {
//     val |= O_NONBLOCK;
//   } else {
//     val &= ~O_NONBLOCK;
//   }

//   if (::fcntl(fd_, F_SETFL, val) == -1) {
//     assert(false);
//   }
// }

// int Socket::SetSockOpt(int opt, const void* val, socklen_t opt_len, int level) {
//   return ::(fd_, level, opt, val, opt_len);
// }

// int Socket::GetSockOpt(int opt, void* val, socklen_t* opt_len, int level) {
//   return ::getsockopt(fd_, level, opt, val, opt_len);
// }

// void Socket::SetNoCloseWait() {
//   struct linger ling;
//   ling.l_onoff = 1;  // 在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
//   ling.l_linger = 0;  // 容许逗留的时间为0秒

//   if (SetSockOpt(SO_LINGER, static_cast<const void*>(&ling), static_cast<socklen_t>(sizeof(linger)),
//                  SOL_SOCKET) == -1) {
//     assert(false);
//   }
// }

// void Socket::SetSendBufferSize(int sz) {
//   int flag = 1;
//   if (SetSockOpt(SO_SNDBUF, static_cast<const void*>(&sz), static_cast<socklen_t>(sizeof(flag)),
//                  SOL_SOCKET) == -1) {
//     assert(false);
//   }
// }

// int Socket::GetSendBufferSize() {
//   int sz = 0;
//   socklen_t len = sizeof(sz);
//   if (GetSockOpt(SO_SNDBUF, static_cast<void*>(&sz), &len, SOL_SOCKET) == -1 || len != sizeof(sz)) {
//     assert(false);
//   }

//   return sz;
// }

// void Socket::SetRecvBufferSize(int sz) {
//   if (SetSockOpt(SO_RCVBUF, static_cast<const void*>(&sz), static_cast<socklen_t>(sizeof(sz)),
//                  SOL_SOCKET) == -1) {
//     assert(false);
//   }
// }

// int Socket::GetRecvBufferSize() {
//   int sz = 0;
//   socklen_t len = sizeof(sz);
//   if (GetSockOpt(SO_RCVBUF, static_cast<void*>(&sz), &len, SOL_SOCKET) == -1 || len != sizeof(sz)) {
//     assert(false);
//   }

//   return sz;
// }

// }  // namespace trpc

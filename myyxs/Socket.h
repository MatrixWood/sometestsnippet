#ifndef _YXS_SOCKET_H_
#define _YXS_SOCKET_H_

#include "InetAddress.h"
#include "SocketsOps.h"

namespace yxs {

class InetAddress;

class Socket {
 public:
  explicit Socket(int sockfd)
    : sockfd_(sockfd)
  {
    //sockets::setNonBlockAndCloseOnExec(sockfd_);
  }

  ~Socket();

  int fd() const { return sockfd_; }

  void bindAddress(const InetAddress& peeraddr);

  void listen();

  int connect(const InetAddress& peeraddr);

  int accept(InetAddress* peeraddr);

  void setReuseAddr(bool on);

  int checkSocket();

  void closeFd();

 private:
  const int sockfd_;
};

} // namespace yxs

#endif
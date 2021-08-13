#include "Socket.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>

namespace yxs {

Socket::~Socket() {
  closeFd();
}

void Socket::bindAddress(const InetAddress& addr) {
  sockets::bindOrDie(sockfd_, addr.getSockAddrInet());
}

void Socket::listen() {
  sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peeraddr) {
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  int connfd = sockets::accept(sockfd_, &addr);
  if (connfd >= 0) {
    peeraddr->setSockAddrInet(addr);
  }

  return connfd;
}

void Socket::closeFd() {
  sockets::close(sockfd_);
}

void Socket::setReuseAddr(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, sizeof optval);
}

int Socket::connect(const InetAddress& peeraddr) {
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  addr = peeraddr.getSockAddrInet();

  int ret = sockets::connect(sockfd_, addr);
  if (ret != 0) {
    return -1;
  }
  return 0;
}

int Socket::checkSocket() {
  int err;
  socklen_t len = static_cast<socklen_t>(sizeof(err));
  if (::getsockopt(sockfd_, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
    return -1;
  }

  return err == 0 ? 0 : -1;
}

}
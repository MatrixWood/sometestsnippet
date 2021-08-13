#include "SocketsOps.h"

#include <sstream>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

namespace socket_details {

typedef struct sockaddr SA;

template<typename To, typename From>
inline To implicit_cast(From const &f) {
    return f;
}

const SA* sockaddr_cast(const struct sockaddr_in* addr) {
  return static_cast<const SA*>(implicit_cast<const void*>(addr));
}

SA* sockaddr_cast(struct sockaddr_in* addr) {
  return static_cast<SA*>(implicit_cast<void*>(addr));
}

}

void sockets::setNonBlockAndCloseOnExec(int sockfd) {
  int flags = ::fcntl(sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  int ret = ::fcntl(sockfd, F_SETFL, flags);

  // close-on-exec
  //flags = ::fcntl(sockfd, F_GETFD, 0);
  //flags |= FD_CLOEXEC;
  //ret = ::fcntl(sockfd, F_SETFD, flags);
}

int sockets::createNonblockingOrDie() {
  int sockfd = ::socket(AF_INET,
                        SOCK_STREAM, // | SOCK_NONBLOCK | SOCK_CLOEXEC,
                        IPPROTO_TCP);
  
  if (sockfd < 0) {
    close(sockfd);
    perror("sockets::createNonblockingOrDie");
    exit(-1);
  }

  return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr_in& addr) {
  int ret = ::bind(sockfd, socket_details::sockaddr_cast(&addr), sizeof(addr));
  if (ret < 0) {
    perror("sockets::bindOrDie");
    exit(-1);
  }
}

void sockets::listenOrDie(int sockfd) {
  int ret = ::listen(sockfd, SOMAXCONN);
  if (ret < 0) {
    perror("sockets::listenOrDie");
    exit(-1);
  }
}

int sockets::connect(int sockfd, struct sockaddr_in addr) {
  socklen_t addrlen = sizeof(addr);
  // while (true) {
    int ret = ::connect(sockfd, (struct sockaddr*)&addr, addrlen);
    //if (ret == 0) {
    //  perror("sockets::connect: connect to server success.");
    //  break;
    //}
    //if (ret == -1) {
    //  if (errno == EINTR) {
    //    perror("sockets::connect: connect interruptted.");
    //    continue;
    //  } else if (errno == EINPROGRESS) {
    //    break;
    //  } else {
    //    return -1;
    //  }
    //}
  // }
  return 0;
}

int sockets::accept(int sockfd, struct sockaddr_in* addr) {
  socklen_t addrlen = sizeof(*addr);

  int connfd = ::accept(sockfd, socket_details::sockaddr_cast(addr), &addrlen);
  setNonBlockAndCloseOnExec(connfd);

  if (connfd < 0) {
    int savedErrno = errno;
    perror("sockets::accept");
    switch (savedErrno) {
      case EAGAIN:
      case ECONNABORTED:
      case EINTR:
      case EPROTO: // ???
      case EPERM:
      case EMFILE: // per-process lmit of open file desctiptor ???
        // expected errors
        errno = savedErrno;
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case ENFILE:
      case ENOBUFS:
      case ENOMEM:
      case ENOTSOCK:
      case EOPNOTSUPP: {
        std::stringstream ss;
        ss << "unexpected error of ::accept " << savedErrno;
        perror(ss.str().c_str());
        break;
      }
      default: {
        std::stringstream ss;
        ss << "unknown error of ::accept " << savedErrno;
        perror(ss.str().c_str());
        break;
      }
    }
  }
  return connfd;
}

void sockets::close(int sockfd) {
  if (::close(sockfd) < 0) {
    perror("sockets::close");
    exit(-1);
  }
}

void sockets::toHostPort(char* buf, size_t size,
                         const struct sockaddr_in& addr) {
  char host[INET_ADDRSTRLEN] = "INVALID";
  ::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof host);
  uint16_t port = sockets::networkToHost16(addr.sin_port);
  snprintf(buf, size, "%s:%u", host, port);
}

void sockets::fromHostPort(const char* ip, uint16_t port,
                           struct sockaddr_in* addr) {
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
    perror("sockets::fromHostPort");
    exit(-1);
  }
}

struct sockaddr_in sockets::getLocalAddr(int sockfd) {
  struct sockaddr_in localaddr;
  bzero(&localaddr, sizeof localaddr);
  socklen_t addrlen = sizeof(localaddr);
  if (::getsockname(sockfd, socket_details::sockaddr_cast(&localaddr), &addrlen) < 0) {
    perror("sockets::getLocalAddr");
    exit(-1);
  }
  return localaddr;
}

int sockets::getSocketError(int sockfd) {
  int optval;
  socklen_t optlen = sizeof optval;

  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
    return errno;
  } else {
    return optval;
  }
}

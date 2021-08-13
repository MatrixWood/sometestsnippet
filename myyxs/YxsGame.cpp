#include "DefineType.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include "Socket.h"
#include "TcpConnection.h"

int createNonblockingOrDie() {
  int sockfd = ::socket(AF_INET,
                        SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                        IPPROTO_TCP);
  
  if (sockfd < 0) {
    close(sockfd);
    perror("sockets::createNonblockingOrDie");
    exit(-1);
  }

  return sockfd;
}

int main() {
  Socket sock(createNonblockingOrDie());

  InetAddress addr(SERVER_ADDR, SERVER_PORT);

  TcpConnection conn(sock, addr);

  conn.connect();
  conn.getMsg();
  
  return 0;
}
#include "DefineType.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include "Socket.h"
#include "TcpConnection.h"

int main() {
  Socket sock(sockets::createNonblockingOrDie());

  InetAddress addr(SERVER_ADDR, SERVER_PORT);

  TcpConnection conn(sock, addr);

  conn.connect();
  conn.getMsg();
  
  return 0;
}
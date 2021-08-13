#include "DefineType.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include "Socket.h"
#include "TcpConnection.h"

int main() {
  InetAddress addr(SERVER_ADDR, SERVER_PORT);

  TcpConnection conn(addr);

  conn.connect();

  std::string msg;
  conn.getMsg(msg);

  std::cout << msg << std::endl;
  
  return 0;
}
#include "DefineType.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include "Socket.h"
#include "TcpConnection.h"
#include "ConfigReader.h"

using namespace yxs;

int main() {

  ConfigReader::setPath("conf.ini");
  std::string ip = ConfigReader::instance()->getString("client", "server_addr", "47.97.181.98");
  uint16_t port = ConfigReader::instance()->getNumber("client", "server_port", 20000);

  InetAddress addr(ip, port);

  TcpConnection conn(addr);

  conn.connect();

  std::string msg;
  conn.getMsg(msg);

  std::cout << msg << std::endl;
  
  return 0;
}
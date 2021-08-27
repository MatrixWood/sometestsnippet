#include "DefineType.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include "Socket.h"
#include "TcpClient.h"
#include "ConfigReader.h"

using namespace yxs;

int main() {
  ConfigReader::setPath("conf/conf.ini");
  std::string ip = ConfigReader::instance()->getString("client", "server_addr", "47.97.181.98");
  uint16_t port = ConfigReader::instance()->getNumber("client", "server_port", 20000);

  InetAddress addr(ip, port);

  TcpClient conn(addr);

  conn.AsyncConnect();

  bool is_recv = false;

  while (!is_recv) {
    int ret = conn.CheckReceivedData();
    if (ret < 0) {
      return -1;
    }

    ret = conn.RecvData();
    if (ret == 1) {
      is_recv = true;
    }

    conn.DecodePackage();

    conn.ShowData();
  }

  return 0;
}
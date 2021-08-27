#ifndef _YXS_TCPCLIENT_H_
#define _YXS_TCPCLIENT_H_

#include "Socket.h"
#include "DefineType.h"
#include "InetAddress.h"
#include <string>

namespace yxs {

class TcpClient {
 public:
  TcpClient(const InetAddress& peeraddr)
    : m_sock(sockets::createNonblockingOrDie()),
      m_addr(peeraddr),
      m_recv(false),
      m_buf("")
  {
  }

  ~TcpClient()
  {}

  int AsyncConnect();

  int CheckReceivedData();

  int GetMsg(std::string& msg);

  int RecvData(std::string& data);

  int DecodePackages();

 private:
  struct timeval SetTimer(int sec, int usec) {
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    return tv;
  }

 private:
  Socket m_sock;
  InetAddress m_addr;
  bool m_recv;
  std::string m_buf;
  std::vector<std::string> m_recvData;
};

}

#endif
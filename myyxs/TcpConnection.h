#ifndef _YXS_TCPCONNECTION_H_
#define _YXS_TCPCONNECTION_H_

#include "Socket.h"
#include "DefineType.h"
#include "InetAddress.h"
#include <string>

class TcpConnection {
 public:
  TcpConnection(const InetAddress& peeraddr)
    : sock_(sockets::createNonblockingOrDie()),
      addr_(peeraddr),
      is_recv_(false)
  {
  }

  ~TcpConnection()
  {}

  int connect();

  int getMsg(std::string& msg);

  int recvData(std::string& data);

  struct timeval setTimer() {
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    return tv;
  }

 private:
  InetAddress addr_;
  bool is_recv_;
  std::string buf_;
  Socket sock_;
};

#endif
#pragma once

#include "Socket.h"
#include "DefineType.h"
#include "InetAddress.h"
#include <string>

class TcpConnection {
 public:
  TcpConnection(Socket sock, const InetAddress& peeraddr)
    : sock_(sock),
      addr_(peeraddr),
      is_recv_(false)
  {}

  ~TcpConnection()
  {}

  int connect();

  int getMsg();

  int recvData();

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
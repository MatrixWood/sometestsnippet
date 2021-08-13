#include "TcpConnection.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

int TcpConnection::connect() {
  int ret = sock_.connect(addr_);
  return ret;
}

int TcpConnection::recvData(std::string& data) {
  std::vector<char> buffer(MAX_BUF_LENGTH);
  int received = 0;
  //do {
    received = ::recv(sock_.fd(), &buffer[0], buffer.size(), 0);
    if (received == -1) { 
      if (!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)) {
        perror("recv error.");
        return -1;
      }
    } else if (received == 0) {
      perror("peer shutdown.");
      return 0;
    } else {
      buf_.append(buffer.cbegin(), buffer.cend());
      data = buf_;
      return received;
    }
  //} while (received == MAX_BUF_LENGTH);

  return received;
}

int TcpConnection::getMsg(std::string& msg) {
  struct timeval tv = setTimer();

  while (!is_recv_) {
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(sock_.fd(), &readset);
    int select_ret = select(sock_.fd() + 1, &readset, nullptr, nullptr, &tv);
    if (select_ret == 0) {
      perror("[select] timeout.");
      continue;
    } else if (select_ret < 0) {
      perror("[select] connect to server error.");
      return -1;
    }

    if (sock_.checkSocket() == 0) {
      int is_set = FD_ISSET(sock_.fd(), &readset);

      if (is_set) {
        int ret = recvData(msg);
        if (ret > 0)
          is_recv_ = true;
      }
    } else {
      perror("connect to server error.");
    }
  }

  return 0;
}
#include "TcpClient.h"
#include "utils/Log.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

namespace yxs {

int TcpClient::AsyncConnect() {
  int ret = m_sock.connect(m_addr);
  return ret;
}

int TcpClient::CheckReceivedData() {
  fd_set readset;
  FD_ZERO(&readset);
  FD_SET(m_sock.fd(), &readset);

  struct timeval tv = SetTimer(1, 0);

  int ret = ::select(m_sock.fd() + 1, &readset, nullptr, nullptr, &tv);
  if (ret == 0) {
    Log::Info("TcpClient::CheckReceivedData: select timeout");
    return 0;
  }
  if (ret < 0) {
    Log::Error("TcpClient::CheckReceivedData: connect to server error.");
    return -1;
  }
  if (ret > 0) {
    if (FD_ISSET(m_sock.fd(), &readset)) {
      return 1;
    }
  }

  return 0;
}

int TcpClient::RecvData() {
  std::vector<char> buffer(MAX_BUF_LENGTH);
  int received = 0;
  received = ::recv(m_sock.fd(), &buffer[0], buffer.size(), 0);
  if (received == -1) { 
    if (!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)) {
      Log::Error("TcpClient::recvData: recv error.");
      return -1;
    }
  } else if (received == 0) {
    Log::Error("TcpClient::recvData: peer shutdown.");
    return 0;
  } else {
    m_buf.append(buffer.cbegin(), buffer.cend());
    return 1;
  }

  return received;
}

int TcpClient::DecodePackage() {
  int buffLen = m_buf.find_first_of("\n");
  while (buffLen != -1) {
    std::string str = m_buf.substr(0, buffLen);
    m_buf = m_buf.substr(buffLen + 1);
    m_recvData.push_back(str);
    buffLen = m_buf.find_first_of("\n");
  }
  return 0;
}

void TcpClient::ShowData() {
  for (auto& it : m_recvData) {
    std::cout << it << std::endl;
  }
}

}
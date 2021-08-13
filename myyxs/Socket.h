#pragma once

class InetAddress;

class Socket {
 public:
  explicit Socket(int sockfd)
    : sockfd_(sockfd)
  {}

  ~Socket();

  int fd() const { return sockfd_; }

  void bindAddress(const InetAddress& peeraddr);

  void listen();

  int connect(const InetAddress& peeraddr);

  int accept(InetAddress* peeraddr);

  void setReuseAddr(bool on);

  int checkSocket();

  void close();

 private:
  const int sockfd_;
};
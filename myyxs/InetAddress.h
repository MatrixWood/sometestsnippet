#ifndef _YXS_INETADDRESS_H_
#define _YXS_INETADDRESS_H_

#include <string>

#include <netinet/in.h>

namespace yxs {

class InetAddress {
 public:
  explicit InetAddress(uint16_t port);

  InetAddress(const std::string& ip, uint16_t port);

  InetAddress(const struct sockaddr_in& addr)
    : addr_(addr)
  {}

  std::string toHostPort() const;

  const struct sockaddr_in& getSockAddrInet() const { return addr_; }

  void setSockAddrInet(const struct sockaddr_in& addr) {
    addr_ = addr;
  }

 private:
  struct sockaddr_in addr_;
};

} // namespace yxs

#endif
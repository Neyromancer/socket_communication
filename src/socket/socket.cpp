/// \file socket.cpp
/// \brief Socket class implementation
/// \author
/// \date 06.10.2018

#include <sys/socket.h>

namespace socket_communication {
namespace socket {

Socket::Socket() : socketfd_{-1}, domain_{0}, type_{0}, protocol_{0} {}

Socket::Socket(uint32_t domain, uint32_t type, uint32_t protocol) {
  CreateSocket(domain, type, protocol);
}

Socket::~Socket() {
  if (Exist())
    close(socketfd_);
  SetSocket(-1);
}

void SetSocket(int32_t socket) {
  if (socket < 0)
    socket = -1;
  socketfd_ = socket;
}

inline int32_t GetSocket() const noexcept {
  return socketfd_;
}

void CreateSocket(uint32_t domain, uint32_t type, uint32_t protocol) {
  // SetDomain(domain);
  // SetType(type);
  // SetProtocol(protocol);
  socketfd_ = socket(domain, type, protocol);
}

inline bool Socket::Exist() const noexcept {
  return socketfd_ != -1;
}

}
}

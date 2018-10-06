/// \file socket.cpp
/// \brief Socket class implementation
/// \author
/// \date 06.10.2018

namespace socket_communication {
namespace socket {

Socket::Socket() : socketfd_{-1}, domain_{0}, type_{0}, protocol_{0} {}

Socket::Socket(uint32_t domain, uint32_t type, uint32_t protocol) {
  CreateSocket(domain, type, protocol);
}

void CreateSocket(uint32_t domain, uint32_t type, uint32_t protocol) {
  // SetDomain(domain);
  // SetType(type);
  // SetProtocol(protocol);
  socketfd_ = socket(domain, type, protocol);
}

bool Socket::Exist() const noexcept {
  return socketfd_ != -1;
}

}
}

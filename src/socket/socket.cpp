/// \file socket.cpp
/// \brief Socket class implementation
/// \author
/// \date 08.10.2018

#include <sys/socket.h>
#include <unistd.h>

#include <utility>

namespace socket_communication {
namespace socket {

Socket::Socket() : socketfd_{-1}, domain_{-1}, type_{-1}, protocol_{-1} {}

Socket::Socket(int32_t &&socketfd) : domain_{-1}, type_{-1}, protocol_{-1} {
  socketfd_ = std::move(socketfd);
}

Socket::Socket(uint32_t domain, uint32_t type, uint32_t protocol) {
  CreateSocket(domain, type, protocol);
}

Socket::~Socket() {
  if (Exist())
    close(socketfd_);
}

void Socket::SetSocket(int32_t &&socket) {
  if (socket < 0)
    socket = -1;
  socketfd_ = std::move(socket);
}

void Socket::CreateSocket(uint32_t domain, uint32_t type, uint32_t protocol) {
  // SetDomain(domain);
  // SetType(type);
  // SetProtocol(protocol);
  socketfd_ = std::move(socket(domain, type, protocol));
}

bool Socket::ShutDown() {
  return true;
}

bool Socket::SetNoBlocking() {
  return true;
}

} // namespace socket
} // namespace socket_communication

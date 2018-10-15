/// \file socket.cpp
/// \brief Socket class implementation
/// \author
/// \date 15.10.2018

#include "socket/socket.h"

#include <sys/socket.h>
#include <unistd.h>

#include <utility>

namespace socket_communication {

Socket::Socket() : socketfd_{-1}, domain_{-1}, type_{-1}, protocol_{-1},
                   is_readable_{false}, is_writable_{false} {}

Socket::Socket(int32_t &&socketfd) : domain_{-1}, type_{-1}, protocol_{-1}, 
                                     is_readable_{false}, is_writable_{false} {
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

void Socket::SetIsReadable(bool is_readable) {
  is_readable_ = is_readable;
}

void Socket::SetIsWritable(bool is_writable) {
  is_writable_ = is_writable;
}

}  // namespace socket_communication

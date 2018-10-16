/// \file connection.cpp
/// \brief Abstract class implementation.
/// \author
/// \date 16.10.2018

#include "connection/connection.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <cstring>
#include <utility>

namespace socket_communication {

Connection::Connection() : socket_{}, is_connected_{false}, ip_{INADDR_ANY} {
  //SetIp("127.0.0.1");
  SetPort(6600);
}

Connection::Connection(const std::string &ip_addr, int32_t port) 
    : socket_{}, is_connected_{false} {
  SetIp(ip_addr);
  SetPort(port);
 // socket_.CreateSocket(AF_INET, SOCK_DGRAM, 0);
}

bool Connection::Connect() {
  return (is_connected_ = true);
}

bool Connection::Disconnect(Socket socket) {
  is_connected_ = false;
  if (!socket.Exist())
   return is_connected_;
 
  socket.~Socket();
  return !is_connected_;
}

bool Connection::Reconnect() {
  if (IsConnected())
    if (!Disconnect(socket_))
      return false;

  return Connect();
}

void Connection::SetIp(const std::string &ip_addr) {
  if (inet_pton(socket_.GetDomain(), ip_addr.c_str(), &ip_) != 1) {
    // check and if fail log this situation
    inet_pton(socket_.GetDomain(), "127.0.0.1", &ip_);
  }
}

void Connection::SetIp(std::string &&ip_addr) {
  if (inet_pton(socket_.GetDomain(), ip_addr.c_str(), &ip_) != 1) {
    // check and if fail log this situation
    inet_pton(socket_.GetDomain(), "127.0.0.1", &ip_);
  }
}

void Connection::SetIp(uint32_t ip_addr) {
  // check ip_addr value
  ip_ = ip_addr;
}

std::string Connection::GetIpName() const noexcept {
  char ip_buf[INET_ADDRSTRLEN]{};
  std::string str{};
  if (!inet_ntop(socket_.GetDomain(), &ip_, ip_buf, INET_ADDRSTRLEN))
    str = std::string(ip_buf);
  return std::move(str);
}

// i want to know if this method called with
// invalid parameter so i will have check here
void Connection::SetPort(int32_t port) {
  if (port > 0 && port < 65535)
    port_ = port;
  else // log here
    port_ = 6600;
}

void Connection::SetSocket(uint32_t domain, uint32_t type, uint32_t protocol) {
  if (socket_.Exist())
    Disconnect(socket_);
  socket_.CreateSocket(domain, type, protocol);
}

}  // namesapce socket_communication

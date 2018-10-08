/// \file connection.cpp
/// \brief Abstract class implementation.
/// \author
/// \date 06.10.2018

#include "connection/connection.h"

#include <arpa/inet.h>
#include <cstring>

namespace socket_communication {
namespace connection {

Connection::Connection() : is_connected_{false}, socket_{} {
  SetIp("127.0.0.1");
  SetPort(6600);
}

Connection::Connection(const std::string &ip_addr, int32_t port) 
    : is_connected_{false} {
  SetIp(ip_addr);
  SetPort(port);
  socket_.CreateSocket(AF_INET, SOCK_DGRAM, 0);
}

bool Connect() {
  is_connected = true;
}

bool Disconnect() {
  is_connected = false;
}

bool Connection::Reconnect() {
  if (IsConnected())
    if (!Disconnect())
      return false;

  return Connect();
}

inline bool Connection::IsConnected() {
  return is_connected;
}

void Connection::SetIp(const std::string &ip_addr) {
  if (inet_pton(socket_.GetDomain(), ip_addr.c_str(), ip_) != 1) {
    // check and if fail log this situation
    inet_pton(socket_.GetDomain(), "127.0.0.1", ip_);
  }
}

void Connection::SetIp(std::string &&ip_addr) {
  if (inet_pton(socket_.GetDomain(), ip_addr.c_str(), ip_) != 1) {
    // check and if fail log this situation
    inet_pton(socket_.GetDomain(), "127.0.0.1", ip_);
  }
}

void Connection::SetIp(uint32_t ip_addr) {
  // check ip_addr value
  ip_ = ip_addr;
}

std::string Connection::GetIp() const noexcept {
  char ip_buf[INET_ADDRSTRLEN] {};
  if (!inet_ntop(socket_.GetDomain(), ip_, INET_ADDRSTRLEN))
    return std::string(ip_buf);
  return std::string();
}

uint32_t Connection::GetIp() const noexcept {
  return ip_;
}

// i want to know if this method called with
// invalid parameter so i will have check here
void Connection::SetPort(int32_t port) {
  if (port > 0 && port < 65535)
    port_ = port;
  else // log here
    port_ = 6600;
}

inline uint16_t Connection::GetPort() const noexcept {
  return port_;
}

void Connection::SetSocket(uint32_t domain, uint32_t type, uint32_t protocol) {
  if (!socket_.Exist())
    socket._CreateSocket(domain, type, protocol);
}

inline Socket Connection::GetSocket() const noexcept {
  return socket_;
}

}  // namespace connection
}  // namesapce socket_communication

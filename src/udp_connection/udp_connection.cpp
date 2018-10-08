/// \file udp_connection.cpp
/// \brief UdpConnection class implementation.
/// \author
/// \date 08.10.2018

#include "udp_connection/udp_connection.h"

#include <arpa/inet.h>
#include <netinet/udp.h>

namespace socket_communication {
namespace udp_connection {

UdpConnection::UdpConnection() {}

UdpConnection::UdpConnection(const std::string &ip_addr, int32_t port)
    : Connection(ip_addr, port), domain_{AF_INET} {}

bool UdpConnection::Connect() override {
  auto result = false;
  memset(&serv, 0, sizeof(serv));
  serv.sin_family = domain_;
  serv.sin_port = htons(GetPort());
  serv.sin_addr.s_addr = GetIp();
  if (!connect(GetSocket(), (struct sockaddr *)&serv, sizeof(serv)))
    result = Connection::Connect();

  return result;
}

bool UdpConnection::Disconnect(const Socket &socket) override {
  if (socket.Exist())
    socket.~Socket();
  return Connection::Disconnect();
}

bool UdpConnection::Send(std::string &data) const override {}

std::string UdpConnection::Receive() const override {}

} // namespace udp_socket
} // namespace socket_communication

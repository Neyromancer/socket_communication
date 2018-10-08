/// \file udp_connection.cpp
/// \brief UdpConnection class implementation.
/// \author
/// \date 08.10.2018

#include "udp_connection/udp_connection.h"

#include <arpa/inet.h>
#include <netinet/udp.h>
#include <sys/types.h>
#include <unistd.h>

namespace socket_communication {
namespace udp_connection {

static void SetSockaddrStruct(struct sockaddr *);

UdpConnection::UdpConnection() {}

UdpConnection::UdpConnection(const std::string &ip_addr, int32_t port)
    : Connection(ip_addr, port), domain_{AF_INET} {}

UdpConnection::~UdpConnection() {}

bool UdpConnection::Connect() override {
  auto result = false;
  SetSockaddrStruct(&serv);
  if (!connect(GetSocket(), (struct sockaddr *)&serv, sizeof(serv)))
    result = Connection::Connect();

  return result;
}

bool UdpConnection::Disconnect(const Socket &socket) override {
  if (socket.Exist())
    socket.~Socket();
  return Connection::Disconnect();
}

bool UdpConnection::Send(std::string &data) const override {
  bool result = false;
  auto data_sz = data.size();
  if (IsConnected()) {
    while (data_sz > 0) {
      // maybe it instead of data.size() i have to set size to
      // a value quel to the size of a datagram but not to
      // the amount of value passed with data. The same amount of data
      // will be sent as it is sent now.
      auto data_sent = send(GetSocket(), data.c_str(), data.size());
      if (data_send < 0)
        break;
      data_sz -= data_sent;
      result = true;
    }
  } else {
    SetSockaddrStruct(&serv);
    while (data_sz > 0) {
      auto data_sent = sendto(GetSocket(), data.c_str(), data.size(), 0,
                              (struct sockaddr *)&serv, sizeof(serv));
      if (data_sz < 0)
        break;

      data_sz -= data_sent;
      result = true;
    }
  }
  return result;
}

std::string UdpConnection::Receive() const override {
  std::string str{};
  return str;  
}

static void SetSockaddrStruct(struct sockaddr *addr) {
  memset(sock_addr, 0, sizeof(*sock_addr));
  sock_addr->sin_family = domain_;
  sock_addr->sin_port = htons(GetPort());
  sock_addr->sin_addr.s_addr = GetIp();
}

} // namespace udp_socket
} // namespace socket_communication

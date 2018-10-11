/// \file udp_connection.cpp
/// \brief UdpConnection class implementation.
/// \author
/// \date 11.10.2018

#include "udp_connection/udp_connection.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/udp.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream> // added for debuggin

namespace socket_communication {

UdpConnection::UdpConnection() : domain_{AF_INET} {
  InitSockaddr();
}

UdpConnection::UdpConnection(const std::string &ip_addr, int32_t port)
    : Connection(ip_addr, port), domain_{AF_INET} {
  InitSockaddr();
}

UdpConnection::~UdpConnection() {}

bool UdpConnection::Connect() {
  if (addr_.sin_port != htons(GetPort()))
    InitSockaddr();

  auto result = false;
  if (!connect(GetSocket().GetSocket(), (struct sockaddr *)&addr_, 
               sizeof(addr_)))
    result = Connection::Connect();

  return result;
}

bool UdpConnection::Accept() {
  if (addr_.sin_port != htons(GetPort()))
    InitSockaddr();

  auto result = false;
  std::cout << "accept" << std::endl;
  return !result;
}

bool UdpConnection::Send(std::string &data) const {
  bool result = false;
  auto data_sz = data.size();
  if (IsConnected()) {
    while (data_sz > 0) {
      // maybe it instead of data.size() i have to set size to
      // a value quel to the size of a datagram but not to
      // the amount of value passed with data. The same amount of data
      // will be sent as it is sent now.
      auto data_sent = send(GetSocket().GetSocket(), data.c_str(), data.size(),
                            0);
      if (data_sent < 0)
        break;
      data_sz -= data_sent;
      result = true;
    }
  } else {
    while (data_sz > 0) {
      auto data_sent = sendto(GetSocket().GetSocket(), data.c_str(), 
                              data.size(), 0, (struct sockaddr *)&addr_, 
                              sizeof(addr_));
      if (data_sz < 0)
        break;

      data_sz -= data_sent;
      result = true;
    }
  }

  return result;
}

std::string UdpConnection::Receive() const {
  std::string str{};
  return str;  
}

void UdpConnection::InitSockaddr() {
  memset(&addr_, 0, sizeof(addr));
  addr_.sin_family = domain_;
  addr_.sin_port = htons(GetPort());
  addr_.sin_addr.s_addr = GetIp();
}

} // namespace socket_communication

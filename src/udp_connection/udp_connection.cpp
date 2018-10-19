/// \file udp_connection.cpp
/// \brief UdpConnection class implementation.
/// \author
/// \date 16.10.2018

#include "udp_connection/udp_connection.h"

extern "C" {
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
}

#include <cerrno>
#include <cstring>
#include <utility>
#include <iostream> // added for debugging

namespace socket_communication {

UdpConnection::UdpConnection() : domain_{AF_INET} {
  InitSockaddr();
}

UdpConnection::UdpConnection(const std::string &ip_addr, int32_t port)
    : Connection(ip_addr, port), domain_{AF_INET} {
  InitSockaddr();
}

bool UdpConnection::Send(const std::string &data) {
  auto result = true;
  auto data_sz = data.size();
  auto total = 0;
  char *data_buf = const_cast<char *>(data.c_str());
  auto data_sent = 0;
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp()) {
    InitSockaddr();
  }

  while (total < data_sz) {
    if ((data_sent = sendto(GetSocket().GetSocket(), data_buf + total, 
                            data_sz - total, 0, (struct sockaddr *)&addr_, 
                            sizeof(addr_))) < 0) {
      result = false;
      break;
    }

    total += data_sent;
  }

  return result;
}

bool UdpConnection::Send(std::string &&data) {
  auto result = true;
  auto data_sz = data.size();
  auto total = 0;
  char *data_buf = const_cast<char *>(data.c_str());
  auto data_sent = 0;
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp()) {
    InitSockaddr();
  }

  while (total < data_sz) {
    if ((data_sent = sendto(GetSocket().GetSocket(), data_buf + total, 
                            data_sz - total, 0, (struct sockaddr *)&addr_, 
                            sizeof(addr_))) < 0) {
      result = false;
      break;
    }

    total += data_sent;
  }

  return result;
}

std::string UdpConnection::Receive() const {
  std::string str{};
  auto data_read = 0;
  const auto kBufSize = 4096;
  char buf[kBufSize] {};
  socklen_t size = sizeof(addr_);
  do {
    if ((data_read = recvfrom(GetSocket().GetSocket(), buf, kBufSize, 
                                MSG_DONTWAIT, (struct sockaddr *)&addr_, &size))
                                < 0)
      break;

    if (data_read >= kBufSize)
      data_read = kBufSize - 1;

    str += std::string(buf, data_read);
   } while (data_read);

  return std::move(str);
}

void UdpConnection::InitSockaddr() {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = domain_;
  addr_.sin_port = htons(GetPort());
  addr_.sin_addr.s_addr = GetIp();
}

}  // namespace socket_communication

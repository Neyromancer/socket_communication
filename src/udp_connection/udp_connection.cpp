/// \file udp_connection.cpp
/// \brief UdpConnection class implementation.
/// \author
/// \date 13.10.2018

#include "udp_connection/udp_connection.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/udp.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

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

UdpConnection::~UdpConnection() {}

bool UdpConnection::Connect() {
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp())
    InitSockaddr();

  if (!GetSocket().Exist()) {
    //throw here
    std::cout << "Socket does not exist" << std::endl;
  }
  auto result = false;
  if (!connect(GetSocket().GetSocket(), (struct sockaddr *)&addr_, 
               sizeof(addr_)))
    result = Connection::Connect();

  return result;
}

bool UdpConnection::Listen() {
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp())
    InitSockaddr(); 

  if (bind(GetSocket().GetSocket(), (struct sockaddr *)&addr_,
           sizeof(addr_)) < 0)
    return false;
    // log errors occured here

  if (listen(GetSocket().GetSocket(), backlog_) < 0)
    return false;
    // log errors occured here

  return true;
}

Socket UdpConnection::Accept() {
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp())
    InitSockaddr();

  if (!GetSocket().Exist()) {
    // throw here
    std::cout << "Socket does not exist" << std::endl;
  }
  
  if (GetSocket().GetSocket() >= FD_SETSIZE) {
    //throw here
    std::cout << "Wrong socket" << std::endl;
  }

  fd_set readfds {};
  //fd_set writefds {}; use later
  FD_ZERO(&readfds);
  //FD_ZERO(&writefds);
  FD_SET(GetSocket().GetSocket(), &readfds);
  //FD_SET(GetSocket().GetSocket(), &writefds);
  struct timeval tv {};
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  
  auto tmp_socket = GetSocket().GetSocket() + 1;
  auto retval = 0;
  if ((retval = select(tmp_socket, &readfds, NULL, NULL, &tv) < 0)) {
    std::cout << "select function failed" << std::endl;
    // log any errors occurred here.

  }

  auto new_socketfd = -1;
  // build a method or a field in Socket class to utilize the following test
  if (FD_ISSET(GetSocket().GetSocket(), &readfds) /*||
      FD_ISSET(GetSocket().GetSocket(), &writefds)*/) {
    socklen_t size = sizeof(addr_);
    if ((new_socketfd = accept(GetSocket().GetSocket(), 
                               (struct sockaddr *)&addr_, &size) < 0)) {
      std::cout << "Couldn't created new socket" << std::endl;
      // log any errors occurred here.
    }
  }

  FD_CLR(GetSocket().GetSocket(), &readfds);
  //FD_CLR(GetSocket().GetSocket(), &writefds);
  return std::move(Socket(std::move(new_socketfd)));
}

bool UdpConnection::Send(const std::string &data) {
  auto result = true;
  auto data_sz = data.size();
  auto total = 0;
  char *data_buf = const_cast<char *>(data.c_str());
  auto data_sent = 0;
  if (IsConnected()) {
    while (total < data_sz) {
      // maybe instead of data.size() i have to set size to
      // a value equel to the size of a datagram but not to
      // the amount of value passed with data. The same amount of data
      // will be sent as it is sent now.
      if ((data_sent = send(GetSocket().GetSocket(), data_buf + total, 
                            data_sz - total, 0)) < 0) {
        result = false;
        break;
      }

      total += data_sent;
    }
  } else {
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
  }

  return result;
}

bool UdpConnection::Send(std::string &&data) {
  bool result = false;
  auto data_sz = data.size();
  auto total = 0;
  char *data_buf = const_cast<char *>(data.c_str());
  auto data_sent = 0;
  if (IsConnected()) {
    while (total < data_sz) {
      // maybe it instead of data.size() i have to set size to
      // a value quel to the size of a datagram but not to
      // the amount of value passed with data. The same amount of data
      // will be sent as it is sent now.
      if ((data_sent = send(GetSocket().GetSocket(), data_buf + total, 
                            data_sz - total, 0)) < 0)
        break;

      total += data_sent;
      result = true;
    }
  } else {
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp()) {
    InitSockaddr();
  }

    while (total < data_sz) {
      if ((data_sent = sendto(GetSocket().GetSocket(), data_buf + total, 
                              data_sz - total, 0, (struct sockaddr *)&addr_, 
                              sizeof(addr_))) < 0)
        break;

      total += data_sent;
      result = true;
    }
  }

  return result;
}

std::string UdpConnection::Receive() const {
  std::string str{};
  auto data_read = 0;
  const auto kBufSize = 4096;
  char buf[kBufSize] {};
  if (IsConnected()) {
    do {
      if ((data_read = read(GetSocket().GetSocket(), buf, kBufSize)) < 0)
        break;

      // log this
      if (data_read >= kBufSize)
        data_read = kBufSize - 1;

      str += std::string(buf, data_read);
    } while (data_read);
  } else {
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
  }

  return std::move(str);
}

void UdpConnection::InitSockaddr() {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = domain_;
  addr_.sin_port = htons(GetPort());
  addr_.sin_addr.s_addr = GetIp();
}

}  // namespace socket_communication

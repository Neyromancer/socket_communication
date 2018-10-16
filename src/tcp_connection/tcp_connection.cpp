/// \file tcp_connection.cpp
/// \brief TcpConnection class implementation.
/// \author
/// \date 15.10.2018

#include "tcp_connection/tcp_connection.h"

#include <arpa/inet.h>
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

TcpConnection::TcpConnection() : domain_{AF_INET} {
  InitSockaddr();
}

TcpConnection::TcpConnection(const std::string &ip_addr, int32_t port)
    : Connection(ip_addr, port), domain_{AF_INET} {
  InitSockaddr();
}

bool TcpConnection::Connect() {
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp())
    InitSockaddr();

  auto result = false;
  if (!GetSocket().Exist()) {
    //throw here
    std::cout << "Socket does not exist" << std::endl;
    result = !Disconnect(GetSocket());
  }

  if (!connect(GetSocket().GetSocket(), (struct sockaddr *)&addr_, 
               sizeof(addr_)))
    result = Connection::Connect();

  return result;
}

bool TcpConnection::Listen() {
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp())
    InitSockaddr(); 

  if (bind(GetSocket().GetSocket(), (struct sockaddr *)&addr_,
           sizeof(addr_)) < 0) {
    return !Disconnect(GetSocket());
    // log errors occured here
  }

  if (listen(GetSocket().GetSocket(), backlog_) < 0) {
    return !Disconnect(GetSocket());
    // log errors occured here
  }

  return true;
}

Socket TcpConnection::Accept() {
  if (addr_.sin_port != htons(GetPort()) ||
      addr_.sin_addr.s_addr != GetIp())
    InitSockaddr();

  if (!GetSocket().Exist()) {
    // throw here
    std::cout << "Socket does not exist" << std::endl;
    Disconnect(GetSocket());
  }
 
  // we have to check this because we use select() 
  if (GetSocket().GetSocket() >= FD_SETSIZE) {
    //throw here
    std::cout << "Wrong socket" << std::endl;
    Disconnect(GetSocket());
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
  if (select(tmp_socket, &readfds, NULL, NULL, &tv) < 0) {
    std::cout << "select function failed" << std::endl;
    // log any errors occurred here.

  }

  auto new_socketfd = -1;
  Socket new_socket {};
  // build a method or a field in Socket class to utilize the following test
  if (FD_ISSET(GetSocket().GetSocket(), &readfds) /*||
      FD_ISSET(GetSocket().GetSocket(), &writefds)*/) {
    socklen_t size = sizeof(addr_);
    if ((new_socketfd = accept(GetSocket().GetSocket(), 
                               (struct sockaddr *)&addr_, &size) < 0)) {
      std::cout << "Couldn't created new socket" << std::endl;
      // log any errors occurred here.
    }
    new_socket.SetSocket(std::move(new_socketfd));
    new_socket.SetIsReadable(true);
  }

  FD_CLR(GetSocket().GetSocket(), &readfds);
  //FD_CLR(GetSocket().GetSocket(), &writefds);
//  return std::move(Socket(std::move(new_socketfd)));
  return std::move(new_socket);
}

bool TcpConnection::Send(const std::string &data) {
  auto result = true;
  auto data_sz = data.size();
  auto total = 0;
  char *data_buf = const_cast<char *>(data.c_str());
  auto data_sent = 0;
  if (IsConnected()) {
    while (total < data_sz) {
      if ((data_sent = send(GetSocket().GetSocket(), data_buf + total, 
                            data_sz - total, 0)) < 0) {
        result = false;
        break;
      }

      total += data_sent;
    }
  }

  return result;
}

bool TcpConnection::Send(std::string &&data) {
  bool result = false;
  auto data_sz = data.size();
  auto total = 0;
  char *data_buf = const_cast<char *>(data.c_str());
  auto data_sent = 0;
  if (IsConnected()) {
    while (total < data_sz) {
      if ((data_sent = send(GetSocket().GetSocket(), data_buf + total, 
                            data_sz - total, 0)) < 0)
        break;

      total += data_sent;
      result = true;
    }
  }

  return result;
}

std::string TcpConnection::Receive() const {
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
  }

  return std::move(str);
}

void TcpConnection::SetBackLog(int32_t backlog) {
  if (backlog > 0)
    backlog_ = backlog;
}

void TcpConnection::InitSockaddr() {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = domain_;
  addr_.sin_port = htons(GetPort());
  addr_.sin_addr.s_addr = GetIp();
}

}  // namespace socket_communication

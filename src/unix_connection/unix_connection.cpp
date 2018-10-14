/// \brief unix_connection.cpp
/// \brief UnixConnection class definition.
/// \author
/// \date 14.10.2018

#include "unix_connection/unix_connection.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream> // added for debugging


#define SOCKET_PATH "/var/run/soc_commun/unix.socket" // according to fhs-2.3

namespace socket_communication {

static bool IsValidPath(const std::string &);

UnixConnection::UnixConnection() : path_{SOCKET_PATH} {
  InitSockAddr();
}

UnixConnection::UnixConnection(const std::string &pathname) {
  SetPath(pathname);
  InitSockAddr();
  SetSocket(AF_UNIX, SOCK_STREAM, 0);
}

bool UnixConnection::Connect() {
  if (std::strncmp(addr_.sun_path, path_.c_str(), path_.size()))
    InitSockAddr();

  if (!GetSocket().Exist()) {
    // throw here
    std::cout << "Socket does not exitst" << std::endl;
  }

  auto result = false;
  if (!connect(GetSocket().GetSocket(), (struct sockaddr *)&addr_,
               sizeof(addr_)))
    result = Connection::Connect();
  
  return result;
}

Socket UnixConnection::Accept() {}

bool UnixConnection::Send(const std::string &data) {
  auto result = true;
  auto data_sz = data.size();
  auto data_buf = data.c_str();
  auto total = 0;
  auto data_sent = 0;
  while (total < data_sz) {
    if ((data_sent = write(GetSocket().GetSocket(), data_buf + total, 
                          data_sz - total)) < 0) {
      result = false;
      break;
    }
    total += data_sent;
  }

  return result;
}

bool UnixConnection::Send(std::string &&data) {
  auto result = true;
  auto data_sz = data.size();
  auto data_buf = data.c_str();
  auto total = 0;
  auto data_sent = 0;
  while (total < data_sz) {
    if ((data_sent = write(GetSocket().GetSocket(), data_buf + total, 
                          data_sz - total)) < 0) {
      result = false;
      break;
    }
    total += data_sent;
  }

  return result; 
}

std::string UnixConnection::Receive() const {
  return std::string{"data received"};
}

void UnixConnection::SetPath(const std::string &pathname) {
  if (IsValidPath(pathname))
    path_ = pathname;
  else 
    path_ = SOCKET_PATH;
}

void UnixConnection::SetPath(std::string &&pathname) {
  if (IsValidPath(pathname))
    path_ = std::move(pathname);
  else 
    path_ = SOCKET_PATH;
}

void UnixConnection::InitSockAddr() {
  memset(&addr_, 0, sizeof(struct sockaddr_un));
  addr_.sun_family = AF_UNIX;
  std::strncpy(addr_.sun_path, path_.c_str(), sizeof(addr_.sun_path) - 1);
}

bool UnixConnection::IsValidPath(const std::string &path) {
  auto result = true;
  if (path.c_str()[path.size()] != '\0')
    result = false;

  if (path.size() <= sizeof(addr_.sun_path))
    result = false;

  if (!std::strstr(path.c_str(), "socket"))
    result = false;

  return result;
}

}  // namespace socket_communication

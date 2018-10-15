/// \brief unix_connection.cpp
/// \brief UnixConnection class definition.
/// \author
/// \date 15.10.2018

#include "unix_connection/unix_connection.h"

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream> // added for debugging


#define SOCKET_PATH "/var/run/soc_commun/unix.socket" // according to fhs-2.3

namespace socket_communication {

static bool IsValidPath(const std::string &);

UnixConnection::UnixConnection() : path_{SOCKET_PATH}, backlog_{3} {
  InitSockAddr();
}

UnixConnection::UnixConnection(const std::string &pathname) : backlog_{3} {
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
    Disconnect(GetSocket());
  }

  auto result = false;
  if (!connect(GetSocket().GetSocket(), (struct sockaddr *)&addr_,
               sizeof(addr_)))
    result = Connection::Connect();

  return result;
}

bool UnixConnection::Listen() {
  if (std::strncmp(addr_.sun_path, path_.c_str(), path_.size()))
    InitSockAddr();

  auto result = true;
  if (bind(GetSocket().GetSocket(), (struct sockaddr *)&addr_, 
           sizeof(addr_))) {
    // throw here
    std::cout << "Failed to bind socket | error " << std::strerror(errno) 
              << std::endl;
    result = !Disconnect(GetSocket());
  }

  if (listen(GetSocket().GetSocket(), backlog_)) {
    // throw here
    std::cout << "Failed to start listening | error " << std::strerror(errno)
              << std::endl;
    result = !Disconnect(GetSocket());
  }

  return result;
}

Socket UnixConnection::Accept() {
  if (std::strncmp(addr_.sun_path, path_.c_str(), path_.size()))
    InitSockAddr(); 

  if (!GetSocket().Exist()) {
    // throw here
    std::cout << "socket does not exist" << std::endl;
    Disconnect(GetSocket());
  }

  if (GetSocket().GetSocket() >= FD_SETSIZE) {
    // throw here
    std::cout << "Socket exceeds FD_SETSIZE" << std::endl;
    Disconnect(GetSocket());
  }

  fd_set readfds {};
  // fd_set writefds {}; use later
  FD_ZERO(&readfds);
  // FD_ZERO(&writefds);
  FD_SET(GetSocket().GetSocket(), &readfds);
  // FD_SET(GetSocket().GetSocket(), &writefds);
  struct timeval tv {};
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  auto tmp_socket = GetSocket().GetSocket() + 1;
  if ((tmp_socket, &readfds, NULL, NULL, &tv) < 0) {
    // log here
    std::cout << "select function failed" << std::endl;
  }

  auto new_socketfd = -1;
  // build a method or a field in the Socket class to unilize the results of the
  // following test
  if (FD_ISSET(GetSocket().GetSocket(), &readfds) /* ||
      FD_ISSET(GetSocket().GetSocket(), &writefds*/) {
    socklen_t size = sizeof(addr_);
    if ((new_socketfd = accept(GetSocket().GetSocket(), 
                               (struct sockaddr *)&addr_, &size)) < 0) { 
      std::cout << "Couldn't create a new socket." << std::endl;
      // log any errors occured here.
    }
  }
  FD_CLR(GetSocket().GetSocket(), &readfds);
  // FD_CLR(GetSocket().GetSocket(), &writefds);

  return std::move(Socket{std::move(new_socketfd)});
}

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
  std::string str{};
  auto data_read = 0;
  const auto kBufSize = 4096;
  char buf[kBufSize] {};
  do {
    if ((data_read = read(GetSocket().GetSocket(), buf, kBufSize)) < 0) 
      break;
      // log this

    if (data_read >= kBufSize)
      data_read = kBufSize - 1;

    str += std::string(buf, data_read);
  } while (data_read);

  return std::move(str);
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

void UnixConnection::SetBacklog(int32_t backlog) {
  if (backlog > 0)
    backlog_ = backlog;
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

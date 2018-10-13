/// \brief unix_connection.cpp
/// \brief UnixConnection class definition.
/// \author
/// \date 13.10.2018

#define SOCKET_PATH "/var/run/soc_commun/unix.socket" // according to fhs-2.3

namespace socket_communication {

static bool IsPathValid(const std::string &path) {}

UnixConnection::UnixConnection() : path_{SOCKET_PATH} {}

UnixConnection::UnixConnection(const std::string &pathname) {
  SetPath(pathname);
}

bool UnixConnection::Connect() {}

Socket UnixConnection::Accept() {}

bool UnixConnection::Send(const std::string &data) {}

bool UnixConnection::Send(std::string &&data) {}

std::string UnixConnection::Receive() const {}

void UnixConnection::SetPath(const std::string &pathname) {
  if (IsPathValid(pathname))
    path_ = pathname;
  else 
    path_ = SOCKET_PATH;
}

void UnixConnection::SetPath(std::string &&pathname) {
  if (IsPathValid(pathname))
    path_ = std::move(pathname);
  else 
    path_ = SOCKET_PATH;
}

static bool IsPathValid(const std::string &path) {}

}

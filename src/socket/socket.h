/// \file socket.h
/// \brief Class which represents socket.
/// \author
/// \date 05.10.2018

#ifndef SOCKET_COMMUNICATION_SOCKET_SOCKET_H_
#define SOCKET_COMMUNICATION_SOCKET_SOCKET_H_

#include <cstdint>

namespace socket_communication {
namespace socket {

class Socket {
 public:
  explicit Socket();

  explicit Socket(int32_t);

  ~Socket();

  Socket(const Socket &) = delete;

  Socket(Socket &&) = default;

  Socket &operator=(const Socket &) = delete;

  Socket &operator=(Socket &&) = default;

  /// \brief Set Socket.
  /// \param[in] socket Socket file descriptor.
  void SetSocket(int32_t socket);

  int32_t GetSocket() const noexcept;

 private:
  int32_t socketfd_;
};

}  // namesapce socket
}  // namespace socket_communication
#endif // SOCKET_COMMUNICATION_SOCKET_SOCKET_H_

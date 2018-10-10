/// \file socket.h
/// \brief Class which represents socket.
/// \author
/// \date 06.10.2018

#ifndef SOCKET_COMMUNICATION_SOCKET_SOCKET_H_
#define SOCKET_COMMUNICATION_SOCKET_SOCKET_H_

#include <cstdint>

namespace socket_communication {
namespace socket {

class Socket {
 public:
  explicit Socket();

  /// \brief Construct socket object.
  /// \param[in] socket Socket.
  explicit Socket(int32_t &&socketfd);

  /// \brief Construct socket object.
  /// \param[in] domain Socket domain.
  /// \param[in] type Socket type.
  /// \param[in] protocol Socket protocol.
  explicit Socket(uint32_t domain, uint32_t type, uint32_t protocol);

  ~Socket();

  Socket(const Socket &) = delete;

  Socket(Socket &&) = default;

  Socket &operator=(const Socket &) = delete;

  Socket &operator=(Socket &&) = default;

  /// \brief Set Socket.
  /// \param[in] socket Socket file descriptor.
  void SetSocket(int32_t &&socket);

  inline const int32_t &GetSocket() const noexcept {
    return socketfd_;
  }

  inline uint32_t GetDomain() const noexcept {
    return domain_;
  }

  /// \brief Create socket object.
  /// \param[in] domain Socket domain.
  /// \param[in] type Socket type.
  /// \param[in] protocol Socket protocol.
  void CreateSocket(uint32_t domain, uint32_t type, uint32_t protocol);

  inline bool Exist() const noexcept {
    return socketfd_ != -1;
  }

  bool ShutDown();

  bool SetNoBlocking();

 private:
  int32_t socketfd_;
  int32_t domain_;
  int32_t type_;
  int32_t protocol_;
};

}  // namesapce socket
}  // namespace socket_communication
#endif // SOCKET_COMMUNICATION_SOCKET_SOCKET_H_

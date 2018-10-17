/// \file socket.h
/// \brief Class which represents socket.
/// \author
/// \date 17.10.2018

#ifndef SOCKET_COMMUNICATION_SOCKET_SOCKET_H_
#define SOCKET_COMMUNICATION_SOCKET_SOCKET_H_

#include <cstdint>

namespace socket_communication {

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
  
  /// \brief Class socket desctructor.
  ~Socket();

  /// \brief Copy constructor.
  /// \param[in] socket Socket class object.
  Socket(const Socket &socket) = default;

  /// \brief Move constructor.
  /// \param[in] socket Socket class object.
  Socket(Socket &&socket) = default;

  /// \brief Copy assignment operator.
  /// \param[in] socket Socket class object.
  /// \return Socket class object.
  Socket &operator=(const Socket &socket) = delete;

  /// \brief Move assignment operator.
  /// \param[in] socket Socket class object.
  /// \return Socket class object.
  Socket &operator=(Socket &&socket) = default;

  /// \brief Set Socket.
  /// \param[in] socket Socket file descriptor.
  void SetSocket(int32_t &&socket);

  /// \brief Get socket file descriptor.
  /// \return Socket file descriptor.
  inline const int32_t &GetSocket() const noexcept {
    return socketfd_;
  }

  /// \brief Get domain.
  /// \return Domain.
  inline uint32_t GetDomain() const noexcept {
    return domain_;
  }

  /// \brief Create socket object.
  /// \param[in] domain Socket domain.
  /// \param[in] type Socket type.
  /// \param[in] protocol Socket protocol.
  void CreateSocket(uint32_t domain, uint32_t type, uint32_t protocol);


  /// \brief Return result of checking if socket exist.
  /// \return Result of the check if socket exist.
  inline bool Exist() const noexcept {
    return socketfd_ != -1;
  }

  bool SetNoBlocking();
  
  /// \brief Set socket condition.
  /// \param[in] is_reading Socket condition.
  void SetIsReadable(bool is_readable);
  
  /// \brief Return if the socket is for reading from.
  /// \return Socket condition.
  bool IsReadable() const noexcept {
    return is_readable_;
  }

   /// \brief Set socket condition.
  /// \param[in] is_writable Socket condition. 
  void SetIsWritable(bool is_writable);
  
  /// \brief Return if the socket is for writing in.
  /// \return Socket condition.
  bool IsWritable() const noexcept {
    return is_writable_;
  }

 private:
  int32_t socketfd_;
  int32_t domain_;
  int32_t type_;
  int32_t protocol_;
  bool is_readable_;
  bool is_writable_;
};

}  // namespace socket_communication

#endif // SOCKET_COMMUNICATION_SOCKET_SOCKET_H_

/// \file connection.h
/// \brief Abstract class which implements connection.
/// \author
/// \date 16.10.2018

#ifndef SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_
#define SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_

#include <cstdint>
#include <string>

#include "socket/socket.h"

namespace socket_communication {

class Connection {
 public:
  Connection();
  
  /// \brief Connection class constructor.
  /// \param[in] ip_addr IP.
  /// \param[in] port Port.
  Connection(const std::string &ip_addr, int32_t port);

  virtual ~Connection() = default;

  Connection(const Connection &) = delete;

  Connection(Connection &&) = default;

  Connection &operator=(const Connection &) = delete;

  Connection &operator=(Connection &&) = default;

  virtual bool Connect();

  virtual bool Disconnect(Socket socket);

  /// \brief Send data.
  /// \param[in] data Data to send.
  /// \return Result of sending data.
  virtual bool Send(const std::string &data) = 0;

  /// \brief Send data.
  /// \param[in] data Data to send.
  /// \return Result of sending data.
  virtual bool Send(std::string &&data) = 0;

  /// \brief Receive data.
  /// \return Received data.
  virtual std::string Receive() const = 0;

  bool Reconnect();

  inline bool IsConnected() const noexcept {
    return is_connected_;
  }

  /// \brief Set IP.
  /// \param[in] ip_addr IP adress (string).
  void SetIp(const std::string &ip_addr);
  void SetIp(std::string &&ip_addr);

  /// \brief Set IP.
  /// \param[in] ip_addr IP adress (32-bit data).
  void SetIp(uint32_t ip_addr);

  /// \brief Return IP address.
  /// \return ip_addr IP adress (string).
  std::string GetIpName() const noexcept;

  /// \brief Return IP address.
  /// \return IP adress (32-bit data).
  inline uint32_t GetIp() const noexcept {
    return ip_;
  }

  /// \brief Set port.
  /// \param[in] port Port.
  void SetPort(int32_t port);

  /// \brief Return port.
  /// \return Port.
  inline uint16_t GetPort() const noexcept {
    return port_;
  }

  void SetSocket(uint32_t domain, uint32_t type, uint32_t protocol);

  inline const Socket &GetSocket() const noexcept {
    return socket_;
  }

 private:
  Socket socket_;
  bool is_connected_;
  uint32_t ip_;
  uint16_t port_;
};

}  // namespace socket_communication

#endif // SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_

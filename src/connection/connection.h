/// \file connection.h
/// \brief Abstract class which implements connection.
/// \author
/// \date 05.10.2018

#ifndef SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_
#define SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_

#include <cstdint>
#include <string>

#include "socket/socket.h"

namespace socket_communication {
namespace connection {

class Connection {
 public:
  Connection();
  
  /// \brief Connection class constructor.
  /// \param[in] ip_addr IP.
  /// \param[in] port Port.
  Connection(const std::string &ip_addr, int32_t port);
  virtual ~Connection();

  Connection(const Connection &) = delete;

  Connection(Connection &&) = default;

  Connection &operator=(Connection &) = delete;

  Connection &operator=(Connection &&) = default;

  virtual bool Connect() = 0;

  virtual bool Disconnect() = 0;

  virtual bool Accept() = 0;

  virtual bool Reconnect() = 0;

  void SetState(State state);

  bool IsConnected() const noexcept;

  /// \brief Set IP.
  /// \param[in] ip_addr IP adress (string).
  void SetIp(const std::string &ip_addr);
  void SetIp(std::string &&ip_addr);

  /// \brief Set IP.
  /// \param[in] ip_addr IP adress (32-bit data).
  void SetIp(uint32_t ip_addr);

  /// \brief Return IP address.
  /// \return ip_addr IP adress (string).
  std::string GetIp() const noexcept;

  /// \brief Return IP address.
  /// \return IP adress (32-bit data).
  uint32_t GetIp() const noexcept;

  /// \brief Set port.
  /// \param[in] port Port.
  void SetPort(int32_t port);

  /// \brief Return port.
  /// \return Port.
  uint16_t GetPort() const noexcept;

  void SetSocket(uint32_t domain, uint32_t type, uint32_t protocol);

  Socket GetScoket() const noexcept;

  enum class State : uint16_t {
    disconnected = 0,
    connected
  };

 private:
  State is_connected_;
  Socket socket_;
  uint32_t ip_;
  uint16_t port_;
};
}  // namespace connection
}  // namespace socket_communication

#endif // SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_

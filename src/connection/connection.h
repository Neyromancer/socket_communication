/// \file connection.h
/// \brief Abstract class which implements connection.
/// \author
/// \date 05.10.2018

#ifndef SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_
#define SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_

#include <cstdint>

namespace socket_communication {
namespace connection {

class Connection {
 public:
  Connection();
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

  enum class State : uint16_t {
    disconnected = 0,
    connected
  };

 private:
  State is_connected_;
};
}  // namespace connection
}  // namespace socket_communication

#endif // SOCKET_COMMUNICATION_CONNECTION_CONNECTION_H_

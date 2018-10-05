/// \file connection.cpp
/// \brief Abstract class implementation.
/// \author
/// \date 05.10.2018

#include "connection/connection.h"

namespace socket_communication {
namespace connection {

Connection::Connection() : is_connected_{State.connected} {}

bool Connection::Reconnect() {
  if (!IsConnected())
    if (!Disconnect())
      return false;

  return Connect();
}

void Connection::SetState(State state) {
  is_connected = state;
}

bool Connection::IsConnected() {
  return is_connected == State.connected;
}

}  // namespace connection
}  // namesapce socket_communication

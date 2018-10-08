/// \file udp_connection.h
/// \brief Class which represents UDP connection.
/// \author
/// \date 08.10.2018

#ifndef SOCKET_COMMUNICATION_UDP_CONNECTION_UDP_CONNECTION_H_
#define SOCKET_COMMUNICATION_UDP_CONNECTION_UDP_CONNECTION_H_

#include "connection/connectin.h"

namespace socket_communication {
namespace udp_connection {
class UdpConnection : public Connection {
 public:
  UdpConnection();

  /// \brief UdpConnection class constructor.
  /// \param[in] ip addr IP.
  /// \param[n] port Port.
  UdpConnection(const std::string &ip_addr, int32_t port);

  ~UdpConnection();

  UdpConnection(const UdpConnection &) = delete;
  
  UdpConnection(UdpConnection &&) = default;

  UdpConnection &operator=(const UdpConnection &) = delete;

  UdpConnection &operator=(UdpConnection &&) =  default;

  bool Connect() override;

  bool Disconnect() override;

  bool Accept override;
};

} // namespace socket_community
} // namespace udp_connection

#endif // SOCKET_COMMUNICATION_UDP_CONNECTION_UDP_CONNECTION_H_



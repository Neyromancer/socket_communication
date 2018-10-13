/// \file udp_connection.h
/// \brief Class which represents UDP connection.
/// \author
/// \date 13.10.2018

#ifndef SOCKET_COMMUNICATION_UDP_CONNECTION_UDP_CONNECTION_H_
#define SOCKET_COMMUNICATION_UDP_CONNECTION_UDP_CONNECTION_H_

#include <sys/socket.h>
#include <netinet/in.h>

#include "connection/connection.h"
#include "socket/socket.h"

namespace socket_communication {

class UdpConnection : public Connection {
 public:
  UdpConnection();

  /// \brief UdpConnection class constructor.
  /// \param[in] ip addr IP.
  /// \param[n] port Port.
  UdpConnection(const std::string &ip_addr, int32_t port);

  /// \brief UdpConnection destructor.
  ~UdpConnection();

  /// \brief UdpConnection copy constructor.
  /// \param[in] udp_connection UdpConnection object.
  UdpConnection(const UdpConnection &udp_connection) = delete;

  /// \brief UdpConnection move constructor.
  /// \param[in] udp_connection UdpConnection object. 
  UdpConnection(UdpConnection &&udp_connection) = default;

  /// \brief UdpConnection copy assignment.
  /// \param[in] udp_connection UdpConnection object. 
  UdpConnection &operator=(const UdpConnection &udp_connection) = delete;

  /// \brief UdpConnection move assignment.
  /// \param[in] udp_connection UdpConnection object. 
  UdpConnection &operator=(UdpConnection &&udp_connectino) =  default;

  /// \brief Setup connection.
  /// \return Result of setting connection.
  bool Connect() override;

  /// \brief Wait for incoming connectoin.
  /// \return result of of listening for incoming connections.
  bool Listen() override;

  /// \brief Accept incoming connection.
  /// \return Socket class object.
  Socket Accept() override;

  /// \brief Send data over connection.
  /// param[in] data Data.
  /// \return Result of sending data over connection.
  bool Send(const std::string &data) override;

  /// \brief Send data over connection.
  /// param[in] data Data.
  /// \return Result of sending data over connection.
  bool Send(std::string &&data) override;
 
  /// \brief Receive data over connection.
  /// \return Received data. 
  std::string Receive() const override;

  void SetDomain(int32_t domain);

 private:
  /// \brief Initialize struct sockaddr_in.
  void InitSockaddr();

  struct sockaddr_in addr_;
  int32_t domain_;
  int32_t backlog_;
};

} // namespace socket_community

#endif // SOCKET_COMMUNICATION_UDP_CONNECTION_UDP_CONNECTION_H_

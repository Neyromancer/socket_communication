/// \file tcp_connection.h
/// \brief Class which represents UDP connection.
/// \author
/// \date 17.10.2018

#ifndef SOCKET_COMMUNICATION_TCP_CONNECTION_TCP_CONNECTION_H_
#define SOCKET_COMMUNICATION_TCP_CONNECTION_TCP_CONNECTION_H_

#include <sys/socket.h>
#include <netinet/in.h>

#include "connection/connection.h"
#include "socket/socket.h"

namespace socket_communication {

class TcpConnection : public Connection {
 public:
  TcpConnection();

  /// \brief TcpConnection class constructor.
  /// \param[in] ip addr IP.
  /// \param[n] port Port.
  explicit TcpConnection(const std::string &ip_addr, int32_t port);

  /// \brief TcpConnection destructor.
  ~TcpConnection() = default;

  /// \brief TcpConnection copy constructor.
  /// \param[in] tcp_connection TcpConnection object.
  TcpConnection(const TcpConnection &tcp_connection) = delete;

  /// \brief TcpConnection move constructor.
  /// \param[in] tcp_connection TcpConnection object. 
  TcpConnection(TcpConnection &&tcp_connection) = default;

  /// \brief TcpConnection copy assignment.
  /// \param[in] tcp_connection TcpConnection object. 
  TcpConnection &operator=(const TcpConnection &tcp_connection) = delete;

  /// \brief TcpConnection move assignment.
  /// \param[in] tcp_connection TcpConnection object. 
  TcpConnection &operator=(TcpConnection &&udp_connectino) =  default;

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

  /// \brief Set domain.
  /// \param[in] domain Domain.
  void SetDomain(int32_t domain);

  /// \brief Set backlog.
  /// \param[in] backlog Backlog.
  void SetBackLog(int32_t backlog);

  /// \brief Return backlog.
  /// \return Backlog.
  int32_t GetBacklog() const noexcept {
    return backlog_;
  }

  /// \brief Sets additional control over closing a socket.
  /// \param[in] socket Socket.
  /// \param[in] how Type of shutdown.
  /// \return Result of closing a socket.
  bool ShutDown(Socket socket, int32_t how);

  /// \brief Sets passed socket in non-blocking state.
  /// \param[in] socket Socket.
  /// \return Result of setting socket in non-blocking state.
  bool SetNonBlocking(Socket socket);

 private:
  /// \brief Initialize struct sockaddr_in.
  void InitSockaddr();

  struct sockaddr_in addr_;
  int32_t domain_;
  int32_t backlog_;
};

}  // namespace socket_community

#endif // SOCKET_COMMUNICATION_TCP_CONNECTION_TCP_CONNECTION_H_

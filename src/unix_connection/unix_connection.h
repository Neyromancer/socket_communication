/// \file unix_connection.h
/// \brief Class which represents Unix connection.
/// \author
/// \date 13.10.2018

#ifndef SOCKET_COMMUNICATION_UNIX_CONNECTION_UNIX_CONNECTION_H_
#define SOCKET_COMMUNICATION_UNIX_CONNECTION_UNIX_CONNECTION_H_

namespace socket_communication {

class UnixConnection : public Connection {
 public:
  /// \brief UnixConnection class constructor.
  explicit UnixConnection();

  /// \brief UnixConnection class constructor.
  /// \param[in] pathname Path to a socket.
  explicit UnixConnection(const std::string &pathname);

  /// \brief UnixConnection destructor.
  ~UnixConnection();

  /// \brief UnixConnection copy constructor.
  /// \param[in] unix_connection UnixConnection object.
  UnixConnection(const UnixConnection &unix_connection) = delete;

  /// \brief UnixConnection move constructor.
  /// \param[in] unix_connection UnixConnection object.
  UnixConnection(UnixConnection &&unix_connection) = default;

  /// \brief UnixConnection copy assignment.
  /// \param[in] unix_connection UnixConnection object.
  UnixConnection &operator=(const UnixConnection &unix_connection) = delete;

  /// \brief UnixConnection move assignment.
  /// \param[in] unix_connection UnixConnection object.
  UnixConnection &operator=(UnixConnection &&unix_connection) = default;

  /// \brief Setup connection.
  /// \return Result of setting connection.
  bool Connect() override;

  /// \brief Accept incoming connection.
  /// \return Socket class object.
  Socket Accept() override;

  /// \brief send data over connection.
  /// \param[in] data data.
  /// \return result of sending data voer connection.
  bool Send(const std::string &data) override;

  /// \brief send data over connection.
  /// \param[in] data data.
  /// \return result of sending data voer connection.
  bool Send(std::string &&data) override;

  /// \brief Receive data over connection.
  /// \return Received data.
  std::string Receive() const override;

  /// \brief set path.
  /// \param[in] path path.
  void SetPath(const std::string &path);

  /// \brief set path.
  /// \param[in] path path.
  void SetPath(std::string &&path);

 private:
  std::string path_;
};

}  // namespace socket_communication

#endif // SOCKET_COMMUNICATION_UNIX_CONNECTION_UNIX_CONNECTION_H_

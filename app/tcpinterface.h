#ifndef TCPINTERFACE_H
#define TCPINTERFACE_H

#include <QObject>
#include <QTcpSocket>

class TCPInterface : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool connecting READ connecting NOTIFY connectingChanged)

public:
  explicit TCPInterface(QObject *parent = nullptr);

  Q_INVOKABLE void connectToHost(const QString &host, int port);
  bool connecting() const;

signals:
  void dataReceived(const QByteArray &data);
  void displayDialog(const QString &title, const QString &details);
  void connectingChanged(bool connecting);

public slots:
  void sendData(const QByteArray &data);

protected slots:
  void handleError(QAbstractSocket::SocketError error);
  void handleStateChanged(QAbstractSocket::SocketState state);
  void handleConnected();
  void handleDisconnected();
  void handleReadyRead();

private:
  QTcpSocket m_socket;
};

#endif // TCPINTERFACE_H

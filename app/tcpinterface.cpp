#include "tcpinterface.h"

TCPInterface::TCPInterface(QObject *parent)
  : QObject(parent)
{
  connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
  connect(&m_socket, &QAbstractSocket::connected, this, &TCPInterface::handleConnected);
  connect(&m_socket, &QAbstractSocket::disconnected, this, &TCPInterface::handleDisconnected);
  connect(&m_socket, &QIODevice::readyRead, this, &TCPInterface::handleReadyRead);
  connect(&m_socket, &QAbstractSocket::stateChanged, this, &TCPInterface::handleStateChanged);
}

void
TCPInterface::connectToHost(const QString &host, int port)
{
  qDebug() << "host " << host << port;
  if (m_socket.isOpen())
    {
      m_socket.close();
    }

  m_socket.connectToHost(host, port);
}

bool
TCPInterface::connecting() const
{
  return m_socket.state() == QTcpSocket::ConnectingState;
}

void
TCPInterface::sendData(const QByteArray &data)
{
  if (m_socket.isOpen())
    {
      // todo why?
      QByteArray framedData = QByteArray::fromHex("000212c02100003c0005");
      framedData += data;
      framedData += QByteArray::fromHex("7a");

      qDebug() << "sending on wifi" << qPrintable(framedData.toHex());

      m_socket.write(framedData);
    }
}

void
TCPInterface::handleError(QAbstractSocket::SocketError error)
{
  qDebug() << "error" << error;
  emit displayDialog(tr("Wifi interface"), tr("Error: %1").arg(m_socket.errorString()));
}

void
TCPInterface::handleStateChanged(QAbstractSocket::SocketState state)
{
  emit connectingChanged(state == QAbstractSocket::ConnectingState);
}

void
TCPInterface::handleConnected()
{
  qDebug() << "connected";
  emit displayDialog(tr("Wifi interface"), tr("Connection successful!"));
}

void
TCPInterface::handleDisconnected()
{
  qDebug() << "disconnected";
  emit displayDialog(tr("Wifi interface"), tr("Connection lost!"));
}

void
TCPInterface::handleReadyRead()
{
  emit dataReceived(m_socket.readAll());
}

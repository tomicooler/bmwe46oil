#include "server.h"

#include <QDebug>

Server::Server()
{
  server.listen(QHostAddress::Any, 35000);
  connect(&server, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

void
Server::handleNewConnection()
{
  QTcpSocket *socket = server.nextPendingConnection();
  connect(socket, &QTcpSocket::readyRead, this, &Server::handleReadyRead);
  connect(socket, &QAbstractSocket::disconnected, socket, &QObject::deleteLater);
}

void
Server::handleReadyRead()
{
  static const auto header = QByteArray::fromHex("000212c02100003c0005");
  static const auto footer = QByteArray::fromHex("7a");

  QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
  QByteArray buffer = socket->readAll();

  qDebug() << "request" << buffer.toHex();
  const QByteArray request = buffer.mid(header.size(), buffer.size() - header.size() - 1);

  static const QByteArray oil = QByteArray::fromHex("12050b031f");
  static const QByteArray dsc = QByteArray::fromHex("b829f1032201f5b5");

  if (request == oil) {
    qDebug() << "sending oil response";
    socket->write(QByteArray::fromHex("122da002c30000000038003899bbb5a7c80188fc4e64359c79887c2f7c55fefefefe1447097605050d2e9086c2")); // oil temp 87.377
  } else if (request == dsc) {
    qDebug() << "sending dsc response";
    //socket->write(QByteArray::fromHex("b8f1290c6201f5010110dd8180810000b7"));  // angle 184.365
    socket->write(QByteArray::fromHex("b8f1290c6201f501728000008091000018")); // -5.13
  } else {
    qDebug() << "nem irtam!";
  }
}

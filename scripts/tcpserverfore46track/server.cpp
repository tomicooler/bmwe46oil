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
  static const QByteArray dsc_steering = QByteArray::fromHex("b829f1032201f5b5");
  static const QByteArray dsc_brake = QByteArray::fromHex("b829f102210645");

  // my guess is that I could query the offsets, and use them in the calculations!
  // steering/braking -20:20 (-5:5)
  // yaw -10:10 (-5:5)
  // latg -0.3:0.3 (-0.1:0.1)
  static const QByteArray dsc_offsets = QByteArray::fromHex("b829f102210241");

  if (request == oil) {
    qDebug() << "sending oil response";
    socket->write(QByteArray::fromHex("122da002c30000000038003899bbb5a7c80188fc4e64359c79887c2f7c55fefefefe1447097605050d2e9086c2")); // oil temp 87.377
  } else if (request == dsc_steering) {
    qDebug() << "sending dsc steering wheel response";
    socket->write(QByteArray::fromHex("b8f1290c6201f5010110dd8180810000b7"));  // angle 184.365
    //socket->write(QByteArray::fromHex("b8f1290c6201f501728000008091000018")); // -5.13
  } else if (request == dsc_brake) {
    qDebug() << "sending dsc brake response";
    //socket->write(QByteArray::fromHex("b8f1290f6106ff00c4dc228122acfe46fff8126f")); // 88.33
    //socket->write(QByteArray::fromHex("b8f1290f61061800c6dcffa5ff65fe46000c126c")); // 0.0
    socket->write(QByteArray::fromHex("b8f1290f6106ff00c5dc0336030bfe460004137c")); // 8.22
  }
  else {
    qDebug() << "nem irtam!";
  }
}

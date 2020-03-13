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

  static const QByteArray dsc_steering = QByteArray::fromHex("b829f1032201f5b5");
  static const QByteArray dsc_brake = QByteArray::fromHex("b829f102210645");

  // my guess is that I could query the offsets, and use them in the calculations!
  // steering/braking -20:20 (-5:5)
  // yaw -10:10 (-5:5)
  // latg -0.3:0.3 (-0.1:0.1)
  static const QByteArray dsc_offsets = QByteArray::fromHex("b829f102210241");

  static const QByteArray motronic_m3_throttle = QByteArray::fromHex("12050b031f");

  if (request == motronic_m3_throttle) {
    qDebug() << "sending motronic ecu response";
    //socket->write(QByteArray::fromHex("1227a0000003b600000000000069858b65073a7b8181a60008000440043303e803f603e8635c73")); // DKP 100 Throttle 3.95
    socket->write(QByteArray::fromHex("1227a0000003b600000000000069858b65073a7c8181a600080001e201dd0223022f0223635ce0")); // DKP 54.7 Throttle 2.17
    //socket->write(QByteArray::fromHex("1227a0035d03660054043d00df66828b660a3a898181a6003600000000000000000000005d5b3a")); // RPM 861
    //socket->write(QByteArray::fromHex("1227a00ceb036600b1024d007966808c6a0e3a888181a600810000000000000100000000635c35")); // RPM 3307
  } else if (request == dsc_steering) {
    qDebug() << "sending dsc steering wheel response";
    socket->write(QByteArray::fromHex("b8f1290c6201f5010110dd8180810000b7"));  // angle 184.365
    //socket->write(QByteArray::fromHex("b8f1290c6201f501728000008091000018")); // -5.13
  } else if (request == dsc_brake) {
    qDebug() << "sending dsc brake response";
    //socket->write(QByteArray::fromHex("b8f1290f6106ff00c4dc228122acfe46fff8126f")); // brake 88.33
    //socket->write(QByteArray::fromHex("b8f1290f61061800c6dcffa5ff65fe46000c126c")); // brake 0.0
    //socket->write(QByteArray::fromHex("b8f1290f6106ff00c5dc0336030bfe460004137c")); // brake 8.22

    //socket->write(QByteArray::fromHex("b8f1290f6106ff00c3dcffa5ff6533d501c3131f")); // brake 8.22 yaw 37.816 latg 0.45
    socket->write(QByteArray::fromHex("b8f1290f6106ff00c4dcffbaff65d296febb1322")); // brake 8.22 yaw -33.134 latg -0.325

  } else if (request == dsc_offsets) {
    qDebug() << "sending dsc offsets response";
    socket->write(QByteArray::fromHex("b8f1290c6102fb62f680fbc007640000b8"));
  }
  else {
    qDebug() << "nem irtam!";
  }
}

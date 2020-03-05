#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server: public QObject
{
  Q_OBJECT

public:
  Server();

public slots:
  void handleNewConnection();
  void handleReadyRead();

private:
  QTcpServer server;
};

#endif // SERVER_H

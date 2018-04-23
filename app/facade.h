#ifndef FACADE_H
#define FACADE_H

#include <QObject>
#include <QTimer>
#include <QList>

#include <ds2request.h>
#include <ds2parser.h>
#include <dataextractor.h>

class Facade : public QObject
{
  Q_OBJECT

  Q_PROPERTY(DS2Request *request READ request CONSTANT FINAL)
  Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

public:
  explicit Facade(QObject *parent = nullptr);

  DS2Request *request();

  int updateInterval() const;
  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

signals:
  void sendData(const QByteArray &data);
  void messageReceived(const DS2Message &message);
  void updateIntervalChanged(int updateInterval);

public slots:
  void dataReceived(const QByteArray &data);
  void setUpdateInterval(int updateInterval);
  void requestData();

private:
  QTimer m_updateTimer;
  DS2Parser m_parser;
  DS2Request m_request;
  QByteArray m_buffer;
};

#endif // FACADE_H

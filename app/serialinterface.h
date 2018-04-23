#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QObject>
#include <QSerialPort>

class SerialInterface : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged)
  Q_PROPERTY(int baud READ baud WRITE setBaud NOTIFY baudChanged)

public:
  explicit SerialInterface(QObject *parent = nullptr);

  Q_INVOKABLE QStringList portNames();
  Q_INVOKABLE QStringList standardBaudRates();
  Q_INVOKABLE void open();

  QString port() const;
  int baud() const;

signals:
  void dataReceived(const QByteArray &data);
  void portChanged(QString port);
  void baudChanged(int baud);
  void displayDialog(const QString &title, const QString &details);

public slots:
  void sendData(const QByteArray &data);
  void setPort(QString port);
  void setBaud(int baud);

protected slots:
  void handleReadyRead();
  void handleError(QSerialPort::SerialPortError error);
  void handleBytesWritten(qint64 written);

private:
  QSerialPort m_serial;
  QString m_port;
};

#endif // SERIALINTERFACE_H

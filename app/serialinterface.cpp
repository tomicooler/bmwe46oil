#include "serialinterface.h"

#include <QDebug>
#include <QSerialPortInfo>

SerialInterface::SerialInterface(QObject *parent)
  : QObject(parent)
{
  QStringList ports = portNames();
  if (ports.size() > 0)
    {
      m_serial.setPortName(ports.at(0));
    }
  m_serial.setBaudRate(QSerialPort::Baud9600);
  m_serial.setDataBits(QSerialPort::Data8);
  m_serial.setStopBits(QSerialPort::OneStop);
  m_serial.setParity(QSerialPort::EvenParity);

  connect(&m_serial, &QIODevice::readyRead, this, &SerialInterface::handleReadyRead);
  connect(&m_serial, &QSerialPort::bytesWritten, this, &SerialInterface::handleBytesWritten);
  connect(&m_serial, &QSerialPort::errorOccurred, this, &SerialInterface::handleError);
}

QStringList
SerialInterface::portNames()
{
  QStringList names;
  foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
      names << port.portName();
    }
  return names;
}

QStringList
SerialInterface::standardBaudRates()
{
  QStringList bauds;
  foreach (qint32 baud, QSerialPortInfo::standardBaudRates())
    {
      bauds << QString::number(baud);
    }
  return bauds;
}

void
SerialInterface::open()
{
 if (!m_serial.open(QSerialPort::ReadWrite))
   {
     emit displayDialog(tr("SerialInterface"), tr("Error: %1").arg(m_serial.errorString()));
     return;
   }

  emit displayDialog(tr("SerialInterface"), tr("Connected!"));
}

QString
SerialInterface::port() const
{
  return m_port;
}

int
SerialInterface::baud() const
{
  return m_serial.baudRate();
}

void
SerialInterface::sendData(const QByteArray &data)
{
  if (m_serial.isOpen())
    {
      m_serial.write(data);
    }
}

void
SerialInterface::setPort(QString port)
{
  if (m_port == port)
    return;

  m_port = port;
  emit portChanged(m_port);
}

void
SerialInterface::setBaud(int baud)
{
  if (m_serial.baudRate() == baud)
    return;

  m_serial.setBaudRate(baud);
  emit baudChanged(m_serial.baudRate());
}

void
SerialInterface::handleReadyRead()
{
  emit dataReceived(m_serial.readAll());
}

void
SerialInterface::handleError(QSerialPort::SerialPortError error)
{
  qDebug() << error;
  emit displayDialog(tr("SerialInterface"), tr("Error: %1").arg(m_serial.errorString()));
}

void
SerialInterface::handleBytesWritten(qint64 written)
{
  qDebug() << "written" << written;
}

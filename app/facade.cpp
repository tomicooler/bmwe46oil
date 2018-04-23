#include "facade.h"

#include <QDebug>

Facade::Facade(QObject *parent)
  : QObject(parent)
{
  m_updateTimer.setInterval(250);
  connect(&m_updateTimer, &QTimer::timeout, this, &Facade::requestData);
}

DS2Request *
Facade::request()
{
  return &m_request;
}

int
Facade::updateInterval() const
{
  return m_updateTimer.interval();
}

void
Facade::start()
{
  m_buffer.clear();
  m_updateTimer.start();
}

void
Facade::stop()
{
  m_updateTimer.stop();
}

void
Facade::dataReceived(const QByteArray &data)
{
  m_buffer += data;

  try
    {
      qDebug() << "receiving" << data.toHex();
      while (!m_buffer.isEmpty())
        {
          DS2Message message = m_parser.parse(m_buffer);
          if (message.data.size() > 0 && message.data.startsWith('\xa0')) // acked
            {
              emit messageReceived(message);
            }
        }
    }
  catch (const DS2Parser::NotEnoughData &e)
    {
      qDebug() << e.what();
    }
  catch (const DS2Parser::InvalidChecksum &e)
    {
      qDebug() << e.what();
      m_buffer.clear();
    }
}

void
Facade::requestData()
{
  qDebug() << "sending" << DS2Parser::serialize(request()->message()).toHex();
  emit sendData(DS2Parser::serialize(request()->message()));
}

void
Facade::setUpdateInterval(int updateInterval)
{
  if (m_updateTimer.interval() == updateInterval)
    return;

  m_updateTimer.setInterval(updateInterval);
  if (m_updateTimer.isActive())
    {
      m_updateTimer.start();
    }

  emit updateIntervalChanged(m_updateTimer.interval());
}

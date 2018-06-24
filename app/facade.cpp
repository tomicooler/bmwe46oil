#include "facade.h"

#include <QDebug>

Facade::Facade(QObject *parent)
  : QObject(parent),
    m_clutch(false),
    m_brake(false),
    m_requester_id(0)
{
  m_updateTimer.setInterval(100);

  // 12 0b04
  {
    m_clutchbrake.config()->setBytepos(1);
    connect(m_clutchbrake.data(), SIGNAL(dataChanged(double)), this, SLOT(clutchbrakeChanged(double)));

    DS2Request *request = new DS2Request();
    request->setEcu("12");
    request->setData("0b04");
    Requester *requester = new Requester(request);
    requester->m_extractors << &m_clutchbrake;
    m_requesters << requester;
  }

  // 12 0b92
  {
    m_throttle.config()->setBytepos(7);
    m_throttle.config()->converter()->setMultiplier(0.466406);
    m_throttle.config()->converter()->setOffset(0.513281);

    DS2Request *request = new DS2Request();
    request->setEcu("12");
    request->setData("0b92");
    Requester *requester = new Requester(request);
    requester->m_extractors << &m_throttle;
    m_requesters << requester;
  }

  {
    DS2Request *request = new DS2Request();
    request->setEcu("57");
    request->setData("00");
    m_requesters << new Requester(request);
  }

  // 57 1b01
  {
    m_steering.config()->setBytepos(1);
    m_steering.config()->setLength(2);
    m_steering.config()->setMsb(true);
    m_steering.config()->converter()->setMultiplier(0.0439457);

    DS2Request *request = new DS2Request();
    request->setEcu("57");
    request->setData("1b01");
    Requester *requester = new Requester(request);
    requester->m_extractors << &m_steering;
    m_requesters << requester;
  }

  {
    DS2Request *request = new DS2Request();
    request->setEcu("57");
    request->setData("00");
    m_requesters << new Requester(request);
  }
}

Facade::~Facade()
{
  qDeleteAll(m_requesters);
}

DS2Request *
Facade::request()
{
  return &m_request;
}

VirtualControl *
Facade::control()
{
  return &m_control;
}

DataExtractor *
Facade::throttle()
{
  return &m_throttle;
}

DataExtractor *
Facade::steering()
{
  return &m_steering;
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
  QTimer::singleShot(50, this, SLOT(requestData()));
}

void
Facade::stop()
{
  m_updateTimer.stop();
}

bool
Facade::clutch() const
{
  return m_clutch;
}

bool
Facade::brake() const
{
  return m_brake;
}

void
Facade::dataReceived(const QByteArray &data)
{
  m_buffer += data;

  try
    {
      while (!m_buffer.isEmpty())
        {
          DS2Message message = m_parser.parse(m_buffer);
          if (message.data.size() > 0 && message.data.startsWith('\xa0')) // acked
            {
              foreach (DataExtractor *extractor, m_requesters[m_requester_id]->m_extractors)
                {
                  extractor->messageReceived(message);
                }

              if (m_updateTimer.isActive()) // that timer is not used :D
                {
                  m_requester_id++;
                  QTimer::singleShot(25, this, SLOT(requestData()));
                }
            }
        }
    }
  catch (const DS2Parser::NotEnoughData &e)
    {
      Q_UNUSED(e);
    }
  catch (const DS2Parser::InvalidChecksum &e)
    {
      Q_UNUSED(e);
      m_buffer.clear();
    }
}

void
Facade::requestData()
{
  if (m_requesters.size() <= 0)
    return;

  if (m_requesters.size() <= m_requester_id)
    m_requester_id = 0;

  QByteArray req = DS2Parser::serialize(m_requesters[m_requester_id]->m_request->message());

  qDebug() << "sending" << qPrintable(req.toHex());
  emit sendData(req);
}

void
Facade::setClutch(bool clutch)
{
  if (m_clutch == clutch)
    return;

  m_clutch = clutch;
  emit clutchChanged(m_clutch);
}

void
Facade::setBrake(bool brake)
{
  if (m_brake == brake)
    return;

  m_brake = brake;
  emit brakeChanged(m_brake);
}

void
Facade::clutchbrakeChanged(double value)
{
  qint8 casted = value;
  setClutch(casted & 0x11);
  setBrake(casted & 0x0C);
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

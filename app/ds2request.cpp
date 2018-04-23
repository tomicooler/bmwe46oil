#include "ds2request.h"

DS2Request::DS2Request(QObject *parent)
  : QObject(parent)
{
}

QString
DS2Request::ecu() const
{
  return m_ecu;
}

QString
DS2Request::data() const
{
  return m_data;
}

DS2Message
DS2Request::message() const
{
  char e = ecu().isEmpty() ? '\x12' : QByteArray::fromHex(ecu().toLatin1()).at(0);
  QByteArray d = data().isEmpty() ? QByteArray("0b03") : QByteArray::fromHex(data().toLatin1());
  return DS2Message{ e, d };
}

void
DS2Request::setEcu(QString ecu)
{
  if (m_ecu == ecu)
    return;

  m_ecu = ecu;
  emit ecuChanged(m_ecu);
}

void
DS2Request::setData(QString data)
{
  if (m_data == data)
    return;

  m_data = data;
  emit dataChanged(m_data);
}

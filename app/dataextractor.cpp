#include "dataextractor.h"

#include <QtEndian>

DataExtractor::DataExtractor(QObject *parent)
  : QObject(parent)
{
}

Config *
DataExtractor::config()
{
  return &m_config;
}

Data *
DataExtractor::data()
{
  return &m_data;
}

void
DataExtractor::messageReceived(const DS2Message &message)
{
  if (m_config.length() == 0)
    return;

  if (message.data.size() <= (m_config.bytepos() + m_config.length()))
    return;

  char input[m_config.length()];
  memcpy(input, message.data.constData() + m_config.bytepos(), m_config.length());

  double number = 0.0;

  switch (m_config.length()) {
    case 1:
      number = quint8(input[0]);
      break;

    case 2:
      number = m_config.msb() ? qFromBigEndian< quint16 >(input) : qFromLittleEndian< quint16 >(input);
      break;

    case 4:
      number = m_config.msb() ? qFromBigEndian< quint32 >(input) : qFromLittleEndian< quint32 >(input);
      break;
    }

  m_data.setData((m_config.converter()->multiplier() * number) + m_config.converter()->offset());
}

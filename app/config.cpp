#include "config.h"

Config::Config(QObject *parent)
  : QObject(parent),
    m_bytepos(0),
    m_length(1),
    m_msb(true)
{
}

int
Config::bytepos() const
{
  return m_bytepos;
}

int
Config::length() const
{
  return m_length;
}

double
Config::msb() const
{
  return m_msb;
}

Converter *
Config::converter()
{
  return &m_converter;
}

void Config::setBytepos(int bytepos)
{
  if (m_bytepos == bytepos)
    return;

  m_bytepos = bytepos;
  emit byteposChanged(m_bytepos);
}

void
Config::setLength(int length)
{
  if (m_length == length)
    return;

  m_length = length;
  emit lengthChanged(m_length);
}

void
Config::setMsb(double msb)
{
  if (qFuzzyCompare(m_msb, msb))
    return;

  m_msb = msb;
  emit msbChanged(m_msb);
}

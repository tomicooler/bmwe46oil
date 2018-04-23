#include "converter.h"

Converter::Converter(QObject *parent)
  : QObject(parent)
{
}

double
Converter::convert(double data) const
{
  return (data * multiplier()) + offset();
}

double
Converter::multiplier() const
{
  return m_multiplier;
}

double
Converter::offset() const
{
  return m_offset;
}

void
Converter::setMultiplier(double multiplier)
{
  if (qFuzzyCompare(m_multiplier, multiplier))
    return;

  m_multiplier = multiplier;
  emit multiplierChanged(m_multiplier);
}

void
Converter::setOffset(double offset)
{
  if (qFuzzyCompare(m_offset, offset))
    return;

  m_offset = offset;
  emit offsetChanged(m_offset);
}

#include "data.h"

Data::Data(QObject *parent)
  : QObject(parent)
{
}

QString
Data::title() const
{
  return m_title;
}

QString
Data::unit() const
{
  return m_unit;
}

double
Data::data() const
{
  return m_data;
}

void
Data::setTitle(QString title)
{
  if (m_title == title)
    return;

  m_title = title;
  emit titleChanged(m_title);
}

void
Data::setUnit(QString unit)
{
  if (m_unit == unit)
    return;

  m_unit = unit;
  emit unitChanged(m_unit);
}

void
Data::setData(double data)
{
  if (qFuzzyCompare(m_data, data))
    return;

  m_data = data;
  emit dataChanged(m_data);
}

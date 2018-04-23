#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>

class Converter : public QObject
{
  Q_OBJECT

  Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier NOTIFY multiplierChanged)
  Q_PROPERTY(double offset READ offset WRITE setOffset NOTIFY offsetChanged)

public:
  explicit Converter(QObject *parent = nullptr);

  Q_INVOKABLE double convert(double data) const;

  double multiplier() const;
  double offset() const;

signals:
  void multiplierChanged(double multiplier);
  void offsetChanged(double offset);

public slots:
  void setMultiplier(double multiplier);
  void setOffset(double offset);

private:
  double m_multiplier;
  double m_offset;
};

#endif // CONVERTER_H

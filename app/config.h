#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

#include <converter.h>

class Config : public QObject
{
  Q_OBJECT

  Q_PROPERTY(int bytepos READ bytepos WRITE setBytepos NOTIFY byteposChanged)
  Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged)
  Q_PROPERTY(double msb READ msb WRITE setMsb NOTIFY msbChanged)
  Q_PROPERTY(Converter *converter READ converter CONSTANT FINAL)

public:
  explicit Config(QObject *parent = nullptr);

  int bytepos() const;
  int length() const;
  double msb() const;
  Converter *converter();

signals:
  void byteposChanged(int bytepos);
  void lengthChanged(int length);
  void msbChanged(double msb);

public slots:
  void setBytepos(int bytepos);
  void setLength(int length);
  void setMsb(double msb);

private:
  int m_bytepos;
  int m_length;
  double m_msb;
  Converter m_converter;
};

#endif // CONFIG_H

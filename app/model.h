#ifndef MODEL_H
#define MODEL_H

#include <QObject>

class Model : public QObject {
  Q_OBJECT

  Q_PROPERTY(double oil READ oil WRITE setOil NOTIFY oilChanged)

public:
  using QObject::QObject;

  double oil() const;

public slots:
  void setOil(double oil);

signals:
  void oilChanged(double oil);

private:
  double m_oil{0.0};
};

#endif // MODEL_H

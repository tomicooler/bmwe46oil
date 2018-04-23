#ifndef DS2REQUEST_H
#define DS2REQUEST_H

#include <QObject>
#include <QString>

#include <ds2message.h>

class DS2Request : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString ecu READ ecu WRITE setEcu NOTIFY ecuChanged)
  Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)

public:
  explicit DS2Request(QObject *parent = nullptr);

  QString ecu() const;
  QString data() const;

  DS2Message message() const;

signals:
  void ecuChanged(QString ecu);
  void dataChanged(QString data);

public slots:
  void setEcu(QString ecu);
  void setData(QString data);

private:
  QString m_ecu;
  QString m_data;
};

#endif // DS2REQUEST_H

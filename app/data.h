#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QString>

class Data : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
  Q_PROPERTY(double data READ data WRITE setData NOTIFY dataChanged)

public:
  explicit Data(QObject *parent = nullptr);

  QString title() const;
  QString unit() const;
  double data() const;

signals:
  void titleChanged(QString title);
  void unitChanged(QString unit);
  void dataChanged(double data);

public slots:
  void setTitle(QString title);
  void setUnit(QString unit);
  void setData(double data);

private:
  QString m_title;
  QString m_unit;
  double m_data;
};

#endif // DATA_H

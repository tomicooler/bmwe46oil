#ifndef DATAEXTRACTOR_H
#define DATAEXTRACTOR_H

#include <QObject>

#include <ds2message.h>
#include <config.h>
#include <data.h>

class DataExtractor : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Config *config READ config CONSTANT FINAL)
  Q_PROPERTY(Data *data READ data CONSTANT FINAL)

public:
  explicit DataExtractor(QObject *parent = nullptr);

  Config *config();
  Data *data();

public slots:
  void messageReceived(const DS2Message &message);

private:
  Config m_config;
  Data m_data;
};

#endif // DATAEXTRACTOR_H

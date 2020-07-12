#ifndef FACADE_H
#define FACADE_H

#include <QObject>
#include <QTimer>

#include "ds2message.h"
#include "requester.h"

class Facade : public QObject {
  Q_OBJECT

  Q_PROPERTY(Model *model READ model CONSTANT)
  Q_PROPERTY(int delay READ delay WRITE setDelay NOTIFY delayChanged)

public:
  explicit Facade(QObject *parent = nullptr);

  Model *model();
  int delay() const;

signals:
  void sendData(const QByteArray &data);
  void delayChanged(int delay);

public slots:
  void sendRequest();
  void dataReceived(const QByteArray &data);
  void connected();
  void responseTimeout();
  void setDelay(int delay);

private:
  void chooseNextRequester();

private:
  std::shared_ptr<Model> m_model;
  QByteArray buffer;
  DS2Parser parser;

  Requester requester;
  bool m_connected{false};
  QTimer responeTimer;
  int m_delay{};
};

#endif // FACADE_H

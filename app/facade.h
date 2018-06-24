#ifndef FACADE_H
#define FACADE_H

#include <QObject>
#include <QTimer>
#include <QList>

#include <memory>

#include <ds2request.h>
#include <ds2parser.h>
#include <dataextractor.h>
#include <virtualcontrol.h>

struct Requester
{
  explicit Requester(DS2Request *request)
    : m_request(request)
  {}

  std::unique_ptr< DS2Request > m_request;
  QList< DataExtractor* > m_extractors;
};

class Facade : public QObject
{
  Q_OBJECT

  Q_PROPERTY(DS2Request *request READ request CONSTANT FINAL)
  Q_PROPERTY(DataExtractor *throttle READ throttle CONSTANT FINAL)
  Q_PROPERTY(DataExtractor *steering READ steering CONSTANT FINAL)
  Q_PROPERTY(VirtualControl *control READ control CONSTANT FINAL)
  Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)
  Q_PROPERTY(bool clutch READ clutch WRITE setClutch NOTIFY clutchChanged)
  Q_PROPERTY(bool brake READ brake WRITE setBrake NOTIFY brakeChanged)

public:
  explicit Facade(QObject *parent = nullptr);
  ~Facade();

  DS2Request *request();
  VirtualControl *control();
  DataExtractor *throttle();
  DataExtractor *steering();

  int updateInterval() const;
  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

  bool clutch() const;
  bool brake() const;

signals:
  void sendData(const QByteArray &data);
  void messageReceived(const DS2Message &message);
  void updateIntervalChanged(int updateInterval);
  void clutchChanged(bool clutch);
  void brakeChanged(bool brake);

public slots:
  void dataReceived(const QByteArray &data);
  void setUpdateInterval(int updateInterval);
  void requestData();
  void setClutch(bool clutch);
  void setBrake(bool brake);

  void clutchbrakeChanged(double value);

private:
  QTimer m_updateTimer;
  DS2Parser m_parser;
  DS2Request m_request;
  QByteArray m_buffer;

  VirtualControl m_control;

  DataExtractor m_clutchbrake;
  DataExtractor m_throttle;
  DataExtractor m_steering;
  QList< Requester* > m_requesters; // ugly memory handling
  bool m_clutch;
  bool m_brake;

  int m_requester_id;
};

#endif // FACADE_H

#include "facade.h"

#include <QDateTime>
#include <QDebug>
#include <QTimer>

namespace {

static const inline QByteArray acknowledgement = QByteArray::fromHex("a0");

bool isResponse(const DS2Message &message) {
  return message.data.startsWith(acknowledgement);
}

} // namespace

Facade::Facade(QObject *parent)
    : QObject(parent), m_model(std::make_shared<Model>()),
      requester{
          DS2Message{QByteArray::fromHex("12"), QByteArray::fromHex("0b03")},
          {std::make_shared<Oil>()},
          m_model,
          1,
          quint8{42}} {
  responeTimer.setInterval(std::chrono::milliseconds{5000});
  connect(&responeTimer, &QTimer::timeout, this, &Facade::responseTimeout);
}

Model *Facade::model() { return m_model.get(); }

int Facade::delay() const { return m_delay; }

void Facade::dataReceived(const QByteArray &data) {
  if (!m_connected)
    return;

  buffer += data;
  if (buffer.size() > 255) {
    buffer.clear();
  }

  if (!buffer.isEmpty()) {
    responeTimer.start();
  }

  do {
    if (const auto message = parser.parse(buffer); message.has_value()) {
      if (isResponse(message.value())) {
        requester.processResponse(message.value());
        QTimer::singleShot(delay(), this, &Facade::sendRequest);
      }
    } else {
      break;
    }
  } while (buffer.size() > 0);
}

void Facade::sendRequest() {
  responeTimer.start();

  emit sendData(requester.get_message().serialized);
}

void Facade::connected() {
  m_connected = true;
  QTimer::singleShot(100, this, &Facade::sendRequest);
}

void Facade::setDelay(int delay) {
  if (m_delay == delay)
    return;

  m_delay = delay;
  emit delayChanged(m_delay);
}

void Facade::responseTimeout() {
  qDebug() << "response timeout, clearing buffer, retrying new request";
  buffer.clear();
  sendRequest();
}

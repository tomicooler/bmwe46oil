#ifndef DS2MESSAGE_H
#define DS2MESSAGE_H

#include <QByteArray>
#include <QList>

#include <optional>

struct DS2Message {
  explicit DS2Message(const QByteArray &ecu, const QByteArray &data)
      : ecu(ecu), length(ecu.size() == 1 ? ecu.size() + 1 + data.size() + 1
                                         : data.size()),
        data(data), checksum(length) {
    for (const auto b : ecu)
      checksum ^= b;
    for (const auto b : data)
      checksum ^= b;
    serialized += ecu;
    serialized += length;
    serialized += data;
    serialized += checksum;
  }

  QByteArray ecu;
  quint8 length{0};
  QByteArray data;
  char checksum{'\0'};
  QByteArray serialized;
};

class DS2Parser {
public:
  std::optional<DS2Message> parse(QByteArray &data) {
    // [ECU ADDRESS] [LENGTH] [DATA1] [DATAN] [CHECKSUM]
    if (data.size() < 3)
      return std::nullopt;

    quint8 ecuSize = [&]() -> quint8 {
      for (const auto &ecu : knownLongEcus) {
        if (data.startsWith(ecu))
          return ecu.size();
      }
      return 1;
    }();

    struct Length {
      quint8 packetSize{};
      quint8 dataSize{};
    };

    if (data.size() < (ecuSize + 1))
      return std::nullopt;

    const Length length = [&]() -> Length {
      quint8 atEcuSize = data.at(ecuSize);
      return ecuSize == quint8{1}
                 ? Length{std::max(atEcuSize, static_cast<quint8>(3)),
                          static_cast<quint8>(atEcuSize - ecuSize - 2)}
                 : Length{static_cast<quint8>(ecuSize + atEcuSize + 2),
                          atEcuSize};
    }();

    if (data.size() < length.packetSize)
      return std::nullopt;

    const QByteArray msg = data.left(length.packetSize);
    data.remove(0, msg.size());

    char checksum = 0;
    for (int i = 0; i < msg.size() - 1; ++i)
      checksum ^= msg.at(i);

    if (checksum != msg.at(msg.size() - 1))
      return std::nullopt;

    return DS2Message{msg.mid(0, ecuSize),
                      msg.mid(ecuSize + 1, length.dataSize)};
  }

private:
  QList<QByteArray> knownLongEcus{QByteArray::fromHex("b829f1"),
                                  QByteArray::fromHex("b8f129")};
};

#endif // DS2MESSAGE_H

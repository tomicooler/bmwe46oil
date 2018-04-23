#include "ds2parser.h"

DS2Message
DS2Parser::parse(QByteArray &data)
{
  // [ECU ADDRESS] [LENGTH] [DATA1] [DATAN] [CHECKSUM]
  if (data.size() < 3)
    throw NotEnoughData(QString("Could not parse ds2 message - invalid (%1)").arg(QString::fromLatin1(data.toHex())));

  int size = data[1];
  if (size == 0)
    {
      size = 3;
    }
  if (data.size() < data[1])
    throw NotEnoughData(QString("Could not parse ds2 message - too small (%1)").arg(QString::fromLatin1(data.toHex())));

  QByteArray msg = data.mid(0, size);
  char checksum = 0;
  for (int i = 0; i < msg.size() - 1; ++i)
    {
      checksum ^= msg[i];
    }

  if (msg[msg.size() - 1] != checksum)
    throw InvalidChecksum(QString("Could not parse ds2 message - invalid checksum (%1)").arg(QString::fromLatin1(msg.toHex())));

  data.remove(0, msg.size());

  return DS2Message{ msg[0], msg[1], msg.mid(2, msg.size() - 3), msg[msg.size() - 1] };
}

QByteArray
DS2Parser::serialize(const DS2Message &message)
{
  QByteArray data;
  data.append(message.ecu);
  data.append(message.length);
  data.append(message.data);
  data.append(message.checksum);
  return data;
}

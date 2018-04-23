#ifndef DS2MESSAGE_H
#define DS2MESSAGE_H

#include <QByteArray>

struct DS2Message
{
  explicit DS2Message(char ecu, const QByteArray &data)
    : ecu(ecu),
      length((3* sizeof(char)) + data.size()),
      data(data),
      checksum(ecu)
  {
    checksum ^= length;
    for (int i = 0; i < data.size(); ++i)
      {
        checksum ^= data.at(i);
      }
  }

  explicit DS2Message(char ecu = 0, char length = 0, const QByteArray &data = {}, char checksum = 0)
    : ecu(ecu),
      length(length),
      data(data),
      checksum(checksum)
  {
  }

  char       ecu       = 0;
  char       length    = 0;
  QByteArray data;
  char       checksum  = 0;
};

#endif // DS2MESSAGE_H

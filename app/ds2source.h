#ifndef DS2SOURCE_H
#define DS2SOURCE_H

#include <ds2message.h>

class DS2Source
{
public:
  virtual ~DS2Source() {}
  virtual void messageReceived(const DS2Message &message) = 0;
}

#endif // DS2SOURCE_H

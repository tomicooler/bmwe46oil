#ifndef DS2PARSER_H
#define DS2PARSER_H

#include <ds2message.h>
#include <exception.h>


class DS2Parser
{
public:
  class NotEnoughData : public Exception
  {
  public:
    using Exception::Exception;
  };

  class InvalidChecksum : public Exception
  {
  public:
    using Exception::Exception;
  };

  static DS2Message parse(QByteArray &data); // throws
  static QByteArray serialize(const DS2Message &message);
};

#endif // DS2PARSER_H

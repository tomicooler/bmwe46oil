#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>
#include <exception>

class Exception : public std::exception
{
public:
  explicit Exception(const QString &message)
    : data(message.toUtf8())
  {
  }

  const char *what() const throw()
  {
    return data.constData();
  }

private:
  QByteArray data;
};

#endif // EXCEPTION_H

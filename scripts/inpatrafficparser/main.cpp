#include <QRegularExpression>
#include <QCoreApplication>
#include <QTextStream>
#include <QByteArray>
#include <QDateTime>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QPair>
#include <QSet>

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

struct DS2Message
{
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

class Data
{
public:
  explicit Data()
  {
  }

  void setTimestamp(qint64 timestamp)
  {
    this->m_timestamp = timestamp;
  }

  void append(const QByteArray &data)
  {
    this->input += data;
  }

  void finish()
  {
    // try to parse request - response from the input

    try
    {
      m_request = parseDS2Message(input);
      m_response = parseDS2Message(input);
      if (!input.isEmpty())
        throw Exception("Unknown data");
    }
    catch (const std::exception &e)
    {
      qDebug() << "skipping input" << e.what();
      input.clear();
      setTimestamp(0);
    }
  }

  qint64 timestamp() const
  {
    return m_timestamp;
  }

  const DS2Message &request() const
  {
    return m_request;
  }

  const DS2Message &response() const
  {
    return m_response;
  }

private:
  DS2Message parseDS2Message(QByteArray &data) const
  {
    // [ECU ADDRESS] [LENGTH] [DATA1] [DATAN] [CHECKSUM]
    if (data.size() < 3)
      throw Exception(QString("Could not parse ds2 message - invalid (%1)").arg(QString::fromLatin1(data.toHex())));

    int size = data[1];
    if (data.size() < data[1])
      throw Exception(QString("Could not parse ds2 message - too small (%1)").arg(QString::fromLatin1(data.toHex())));

    QByteArray msg = data.mid(0, size);
    char checksum = 0;
    for (int i = 0; i < msg.size() - 1; ++i)
      {
        checksum ^= msg[i];
      }

    if (msg[msg.size() - 1] != checksum)
      throw Exception(QString("Could not parse ds2 message - invalid checksum (%1)").arg(QString::fromLatin1(msg.toHex())));

    data.remove(0, msg.size());

    return DS2Message{ msg[0], msg[1], msg.mid(2, msg.size() - 3) , msg[msg.size() - 1] };
  }

private:
  qint64     m_timestamp   = 0;
  QByteArray input;
  DS2Message m_request;
  DS2Message m_response;
};


using Message = QPair< qint64, DS2Message >;
using Messages = QVector< QPair< qint64, DS2Message > >;

Messages parseFile(const QString &filename)
{
  QFile file(filename);
  if (!file.open(QFile::ReadOnly))
    throw Exception(file.errorString());

  //  EXAMPLE INPUT:

  //  4530780: 2018-04-15 12:49:06,4906418 +0,0000000

  //   12 04 00 16 12 2E A0 37 35 34 35 31 35 30 31 39   ......�754515019
  //   30 30 31 35 36 30 33 33 30 34 30 30 30 30 31 31   0015603304000011
  //   35 38 35 32 38 31 30 31 00 34 33 33 33 30 33 30   58528101.4333030
  //   37 A4                                             7�

  QTextStream stream(&file);
  QRegularExpression header("[0-9]+: ([0-9]+.[0-9]+.[0-9]+ [0-9]+.[0-9]+.[0-9]+).*");

  Messages messages;
  Data data;

  do
    {
      QString line = stream.readLine().trimmed();
      if (line.isEmpty())
        continue;

      QRegularExpressionMatch match = header.match(line);
      if (match.hasMatch())
        {
          if (data.timestamp() != 0)
            {
              data.finish();

              // filter for the proper sensor data
              // status (response first byte)
              // a0 -> ack
              // a1 -> ecu busy
              // ff -> bad command
              if (data.request().data == QByteArray::fromHex("1b01") &&
                  data.response().data.startsWith('\xa0'))
                {
                  qDebug() << qPrintable(QByteArray((const char*)&data.request().ecu, 1).toHex()) << qPrintable(data.request().data.toHex()) << qPrintable(data.response().data.toHex());
                  messages << Message{ data.timestamp(), data.response() };
                }
            }

          data.setTimestamp(QDateTime::fromString(match.captured(1), "yyyy-MM-dd HH:mm:ss").toSecsSinceEpoch());
        }
      else
        {
          data.append(QByteArray::fromHex(line.split("   ").first().replace(" ", "").toLatin1()));
        }
    }
  while (!stream.atEnd());

  return messages;
}

using Timestamps = QSet< qint64 >;

Timestamps parseTimestamps(const QString &filename)
{
  Timestamps timestamps;

  QFile file(filename);
  if (!file.open(QFile::ReadOnly))
    throw Exception(file.errorString());

  QTextStream stream(&file);
  do
    {
      QStringList columns = stream.readLine().split(",");
      if (columns.size() < 2)
        continue;

      bool ok;
      qint64 timestamp = columns.first().toLongLong(&ok);
      if (!ok)
        continue;

      timestamps << timestamp;
    }
  while (!stream.atEnd());

  return timestamps;
}

int
main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  if (argc != 3)
    {
      qDebug() << QString("USAGE: %1 input.txt timestamps.csv").arg(a.arguments().first());
      return 1;
    }


  Timestamps timestamps = parseTimestamps(a.arguments()[2]);
  qDebug() << timestamps.size();

  Messages messages = parseFile(a.arguments()[1]);
  qDebug() << messages.size();


  QTextStream stream(stdout);
  foreach (const Message &message, messages)
    {
      if (!timestamps.contains(message.first))
        continue;

      timestamps.remove(message.first);

      stream << message.first;
      for (int i = 0; i < message.second.data.size(); ++i)
        {
          stream << "," << static_cast< quint8 >(message.second.data[i]);
        }
      stream << endl;
    }

  return 0;
}

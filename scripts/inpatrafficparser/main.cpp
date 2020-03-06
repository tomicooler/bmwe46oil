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
  QByteArray ecu;
  quint8     length{0};
  QByteArray data;
  char       checksum{'\0'};
};

bool operator==(const DS2Message &a, const DS2Message &b)
{
  return a.ecu == b.ecu && a.length && b.length && a.data == b.data && a.checksum == b.checksum;
}

QString toString(const DS2Message &msg)
{
  QByteArray data;
  data.append(msg.ecu).append(msg.length).append(msg.data).append(msg.checksum);
  return QString::fromLatin1(data.toHex());
}

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
  static DS2Message parseDS2Message(QByteArray &data)
  {
    // [ECU ADDRESS] [LENGTH] [DATA1] [DATAN] [CHECKSUM]
    quint8 ecu_size = data.startsWith(QByteArray::fromHex("b829f1")) || data.startsWith(QByteArray::fromHex("b8f129")) ? 3 : 1;
    if (data.size() < ecu_size + 2)
      throw Exception(QString("Could not parse ds2 message - invalid (%1)").arg(QString::fromLatin1(data.toHex())));

    quint8 packet_size = data.at(ecu_size);
    quint8 data_size = packet_size - ecu_size - 1;

    if (ecu_size > 1) {
        data_size = data[ecu_size];
        packet_size = ecu_size + data_size + 2;
    }

    if (data.size() < data_size)
      throw Exception(QString("Could not parse ds2 message - too small (%1)").arg(QString::fromLatin1(data.toHex())));

    char checksum = 0;
    for (int i = 0; i < packet_size - 1; ++i)
      {
        checksum ^= data.at(i);
      }

    if (data.at(packet_size - 1) != checksum)
      throw Exception(QString("Could not parse ds2 message - invalid checksum (%1)").arg(QString::fromLatin1(data.left(packet_size).toHex())));

    QByteArray raw_packet = data.left(packet_size);
    data.remove(0, raw_packet.size());

    return DS2Message{
          raw_packet.mid(0, ecu_size),
          ecu_size == 1 ? packet_size : data_size,
          raw_packet.mid(ecu_size + 1, data_size),
          raw_packet.at(raw_packet.size() - 1)
    };
  }

private:
  qint64     m_timestamp   = 0;
  QByteArray input;
  DS2Message m_request;
  DS2Message m_response;
};


using Messages = QVector< Data >;

Messages parseDeviceMonitoringStudioFile(const QString &filename)
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
                  messages << data;
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

Messages parseTsharkFile(const QString &filename)
{
  QFile file(filename);
  if (!file.open(QFile::ReadOnly))
    throw Exception(file.errorString());

  //  EXAMPLE INPUT:

  // 000212c02100003c00[1byte: ds2 request length][lenght byte: ds2 response data][1byte: checksum simple c = 0; for b in bytes: c += b;]

  // 1583342581.057130857	000212c02100003c0007b829f10221024170
  // 1583342581.206739591	b8f1290c6102fb66f683fbc007640000bf

  QTextStream stream(&file);

  Messages messages;

  Data req_resp;
  QByteArray request_framing = QByteArray::fromHex("000212c02100003c00");

  bool first = true;
  int count = 0;

  const QChar tab('\t');
  do
    {
      QString line = stream.readLine().trimmed();
      if (line.isEmpty())
        continue;

      QStringList splitted = line.split(tab);

      if (splitted.count() == 2)
        {
          bool ok;
          qint64 timestamp = splitted.at(0).toDouble(&ok); // narrowing
          if (!ok)
            continue;

          req_resp.setTimestamp(timestamp);

          QByteArray data = QByteArray::fromHex(splitted.at(1).toUtf8());

          const bool is_request = data.startsWith(request_framing);
          if (first && !is_request)
            continue;

          first = false;

          ++count;
          if (is_request)
            {
              data = data.mid(request_framing.size() + 1);
              data = data.left(data.size() - 1);
              req_resp.append(data);
            }
          else
            {
              req_resp.append(data);
            }

          if ((count % 2) == 0)
            {
              req_resp.finish();
              messages.append(req_resp);
            }
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

  if (argc != 4)
    {
      qDebug() << QString("USAGE: %1 input.txt timestamps.csv wireshark|devicemonitoringstudio").arg(a.arguments().first());
      qDebug() << "--- input.txt ---";
      qDebug() << "  a) Convert the wireshark pcapng stream:";
      qDebug() << "    input.txt: tshark -r input.pcapng -T fields -e frame.time_epoch -e data &> input.txt";
      qDebug() << "  b) Use the Device Monitoring Studio file";
      qDebug() << "--- timestamps.csv ---";
      qDebug() << "  ./screenshoter.sh # run this script while recording the traffic with wireshark or device m s";
      qDebug() << "  cd imagedirectory && CROP=\"70x24+447+232\" ./ocr-screen.sh";
      return 1;
    }


  Timestamps timestamps = parseTimestamps(a.arguments()[2]);
  qDebug() << timestamps.size();

  Messages messages = a.arguments()[3] == QLatin1String("devicemonitoringstudio") ? parseDeviceMonitoringStudioFile(a.arguments()[1]) : parseTsharkFile(a.arguments()[1]);
  qDebug() << messages.size();



  QStringList rows;
  QMultiHash< qint64, DS2Message > already_on_timestamp;
  for (Messages::const_reverse_iterator it = messages.rbegin(); it != messages.rend(); ++it) // reverse so latest greatest timestamp matching
    {
      const Data &data = *it;
      if (!timestamps.contains(data.timestamp()))
        continue;

      if (already_on_timestamp.values(data.timestamp()).contains(data.request()))
        continue;

      already_on_timestamp.insertMulti(data.timestamp(), data.request());

      QString row;
      QTextStream stream(&row);
      stream << toString(data.request()) << ",";
      stream << data.timestamp();
      for (int i = 0; i < data.response().data.size(); ++i)
        {
          stream << "," << static_cast< quint8 >(data.response().data[i]);
        }
      rows.prepend(row);
    }

  QTextStream stream(stdout);
  for (const auto &row : rows)
    {
      stream << row << '\n';
    }

  return 0;
}

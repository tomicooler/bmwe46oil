#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

#ifndef Q_OS_ANDROID
#include <serialinterface.h>
#endif

#include <dataextractor.h>
#include <tcpinterface.h>
#include <facade.h>

int
main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  qRegisterMetaType< DS2Message >("DS2Message");

#ifndef Q_OS_ANDROID
  qmlRegisterType< SerialInterface >("com.tomi.bmwe46oil", 1, 0, "SerialInterface");
#endif

  qmlRegisterType< VirtualControl >("com.tomi.bmwe46oil", 1, 0, "VirtualControl");
  qmlRegisterType< DataExtractor >("com.tomi.bmwe46oil", 1, 0, "DataExtractor");
  qmlRegisterType< TCPInterface >("com.tomi.bmwe46oil", 1, 0, "TCPInterface");
  qmlRegisterType< DS2Request >("com.tomi.bmwe46oil", 1, 0, "DS2Request");
  qmlRegisterType< Converter >("com.tomi.bmwe46oil", 1, 0, "Converter");
  qmlRegisterType< Facade >("com.tomi.bmwe46oil", 1, 0, "Facade");
  qmlRegisterType< Config >("com.tomi.bmwe46oil", 1, 0, "Config");
  qmlRegisterType< Data >("com.tomi.bmwe46oil", 1, 0, "Data");

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}

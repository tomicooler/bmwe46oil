#include "virtualcontrol.h"

#include <QFileInfo>
#include <QDebug>
#include <QDir>

VirtualControl::VirtualControl(QObject *parent)
  : QObject(parent)
{
  QFileInfo info(QString("%1").arg(__FILE__));
  connect(&m_process, SIGNAL(readyRead()), this, SLOT(readyRead()));
  m_process.start(info.absoluteDir().path() + "/virtualcontrol/build/install/virtualcontrol/bin/virtualcontrol", QStringList{}, QIODevice::ReadWrite | QIODevice::Unbuffered);
  if (!m_process.waitForStarted())
    {
      qDebug() << "ERROR" << m_process.errorString();
    }
}

VirtualControl::~VirtualControl()
{
  m_process.kill();
  m_process.waitForFinished();
}

void
VirtualControl::pressUp()
{
  writeCommand("up");
}

void
VirtualControl::pressDown()
{
  writeCommand("down");
}

void
VirtualControl::pressLeft()
{
  writeCommand("left");
}

void
VirtualControl::pressRight()
{
  writeCommand("right");
}

void
VirtualControl::readyRead()
{
  qDebug() << qPrintable(m_process.readAll());
}

void
VirtualControl::writeCommand(const QByteArray &command)
{
  qDebug() << m_process.state() << command;
  m_process.write(command);
  m_process.write("\n");
  m_process.waitForBytesWritten();
}

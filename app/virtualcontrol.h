#ifndef VIRTUALCONTROL_H
#define VIRTUALCONTROL_H

#include <QObject>
#include <QProcess>

class VirtualControl : public QObject
{
  Q_OBJECT
public:
  explicit VirtualControl(QObject *parent = nullptr);
  ~VirtualControl();

public slots:
  void pressUp();
  void pressDown();
  void pressLeft();
  void pressRight();
  void releaseUp();
  void releaseDown();
  void releaseLeft();
  void releaseRight();

protected slots:
  void readyRead();

private:
  void writeCommand(const QByteArray &command);

private:
  QProcess m_process;
};

#endif // VIRTUALCONTROL_H

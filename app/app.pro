QT += quick network svg
CONFIG += c++11
TARGET = E46Oil

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    converter.h \
    data.h \
    config.h \
    ds2request.h \
    ds2message.h \
    ds2parser.h \
    exception.h \
    ds2source.h \
    dataextractor.h \
    facade.h \
    tcpinterface.h \
    virtualcontrol.h

SOURCES += \
    main.cpp \
    converter.cpp \
    data.cpp \
    config.cpp \
    ds2request.cpp \
    ds2parser.cpp \
    dataextractor.cpp \
    facade.cpp \
    tcpinterface.cpp \
    virtualcontrol.cpp

!android {

QT += serialport

HEADERS += \
    serialinterface.h

SOURCES += \
    serialinterface.cpp

}


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(vendor/quickflux/quickflux.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

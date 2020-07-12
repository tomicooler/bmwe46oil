cmake_minimum_required(VERSION 3.5)

project(app LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
endif()

find_package(Qt5 COMPONENTS Core Quick QuickControls2 Network Svg LinguistTools REQUIRED)

set(TS_FILES app_en_US.ts)

if(ANDROID)
    add_library(app SHARED
      main.cpp
      qml.qrc
      tcpinterface.cpp
      tcpinterface.h
      ds2message.h
      extractors.h
      requester.h
      facade.cpp
      facade.h
      model.h
      model.cpp
      ${TS_FILES}
    )
else()
    add_executable(app
      main.cpp
      qml.qrc
      serialinterface.h
      serialinterface.cpp
      tcpinterface.cpp
      tcpinterface.h
      ds2message.h
      extractors.h
      requester.h
      facade.cpp
      facade.h
      model.h
      model.cpp
      ${TS_FILES}
    )

find_package(Qt5 COMPONENTS SerialPort REQUIRED)
target_link_libraries(app
  PRIVATE Qt5::SerialPort)

endif()

target_compile_definitions(app
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(app
  PRIVATE Qt5::Core Qt5::Quick Qt5::Network Qt5::QuickControls2 Qt5::Svg)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

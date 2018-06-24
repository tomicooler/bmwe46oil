import QtQuick 2.4
import QtQuick.Controls.Material 2.2

Rectangle {
    id: wheel

    property color backgroundColor: "black"
    property color indicatorColor: "red"

    width: 80
    height: width
    radius: width / 2
    color: "transparent"
    border.width: width / 10
    border.color: backgroundColor
    antialiasing: true

    anchors.horizontalCenter: parent.horizontalCenter

    Rectangle {
        width: wheel.width
        height: wheel.border.width
        color: backgroundColor
        antialiasing: true

        anchors.verticalCenter: wheel.verticalCenter
    }

    Rectangle {
        width: wheel.border.width
        height: wheel.height / 2
        color: backgroundColor
        antialiasing: true

        anchors.horizontalCenter: wheel.horizontalCenter
        anchors.bottom: wheel.bottom
    }

    Rectangle {
        width: wheel.width / 3
        height: wheel.height / 3
        color: backgroundColor
        antialiasing: true

        radius: width / 2

        anchors.centerIn: parent
    }

    Rectangle {
        width: wheel.border.width
        height: wheel.border.width
        color: indicatorColor
        antialiasing: true

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }
}

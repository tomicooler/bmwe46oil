import QtQuick 2.4
import QtQuick.Controls.Material 2.2

Rectangle {
    width: 15
    height: 25
    radius: width / 4

    property alias positionIndicator: positionIndicator
    property color backgroundColor: "black"
    property color indicatorColor: "red"

    color: backgroundColor

    Rectangle {
        id: positionIndicator

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width / 5
        height: 0
        color: indicatorColor
    }
}

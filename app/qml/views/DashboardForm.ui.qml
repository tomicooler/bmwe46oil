import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Page {
    width: 600
    height: 400
    property alias buttonRight: buttonRight
    property alias buttonDown: buttonDown
    property alias buttonLeft: buttonLeft
    property alias buttonUp: buttonUp
    property alias steeringWheel: steeringWheel
    property alias throttle: throttle
    property alias brake: brake
    property alias clutch: clutch

    property alias powerButton: powerButton

    title: qsTr("BMW E46 Oil")

    Switch {
        id: powerButton
        text: checked ? qsTr("ON") : qsTr("OFF")
        checked: false
        hoverEnabled: true
        ToolTip.text: qsTr("First connect to WiFi adapter or SerialPort!")
        ToolTip.visible: hovered
    }

    Item {
        id: wheelbase

        width: parent.width
        height: 150
        anchors.centerIn: parent

        SteeringWheel {
            id: steeringWheel
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            anchors.top: steeringWheel.bottom
            anchors.horizontalCenter: wheelbase.horizontalCenter
            anchors.topMargin: 20
            spacing: 10

            Pedal {
                id: clutch
            }

            Pedal {
                id: brake
            }

            Pedal {
                id: throttle
            }
        }
    }

    GridLayout {
        anchors.top: wheelbase.bottom
        anchors.horizontalCenter: wheelbase.horizontalCenter
        anchors.topMargin: 20
        columns: 3
        Rectangle {
            width: 2
            height: 2
            color: "transparent"
        }
        Button {
            id: buttonUp
            text: qsTr("▲")
            checkable: true
        }
        Rectangle {
            width: 2
            height: 2
            color: "transparent"
        }
        Button {
            id: buttonLeft
            text: qsTr("◀")
            checkable: true
        }
        Button {
            id: buttonDown
            text: qsTr("▼")
            checkable: true
        }
        Button {
            id: buttonRight
            text: qsTr("▶")
            checkable: true
        }
    }
}

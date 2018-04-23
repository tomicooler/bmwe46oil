import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Page {
    width: 600
    height: 400
    property alias msbSwitch: msbSwitch
    property alias offsetField: offsetField
    property alias multiplierField: multiplierField
    property alias lengthField: lengthField
    property alias bytePosField: bytePosField
    property alias updateField: updateField
    property alias dataField: dataField
    property alias ecuField: ecuField

    title: qsTr("Configuration")

    Flickable {
        id: flick
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 20
        contentWidth: layout.width
        contentHeight: layout.height
        clip: true

        ScrollBar.horizontal: ScrollBar {
            active: flick.moving || !flick.moving
        }
        ScrollBar.vertical: ScrollBar {
            active: flick.moving || !flick.moving
        }

        ColumnLayout {
            id: layout
            spacing: 20

            GroupBox {
                title: qsTr("Request")
                GridLayout {
                    columns: 2
                    Label {
                        text: qsTr("ECU:")
                    }
                    TextField {
                        id: ecuField
                        placeholderText: qsTr("12")
                    }
                    Label {
                        text: qsTr("Request data:")
                    }
                    TextField {
                        id: dataField
                        placeholderText: qsTr("0b03")
                    }
                    Label {
                        text: qsTr("Update interval (ms):")
                    }
                    SpinBox {
                        id: updateField
                        from: 100
                        to: 1000
                        value: 250
                        stepSize: 50
                    }
                }
            }
            GroupBox {
                title: qsTr("Oil temperature")
                GridLayout {
                    columns: 2
                    Label {
                        text: qsTr("Byte pos:")
                    }
                    TextField {
                        id: bytePosField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        placeholderText: qsTr("12")
                    }
                    Label {
                        text: qsTr("Length:")
                    }
                    SpinBox {
                        id: lengthField
                        from: 1
                        to: 4
                    }
                    Label {
                        text: qsTr("MSB:")
                    }
                    Switch {
                        id: msbSwitch
                        checked: true
                        enabled: lengthField.value > 1
                    }
                    Label {
                        text: qsTr("Multiplier:")
                    }
                    TextField {
                        id: multiplierField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        placeholderText: qsTr("0.796098")
                    }
                    Label {
                        text: qsTr("Offset:")
                    }
                    TextField {
                        id: offsetField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        placeholderText: qsTr("-48.0137")
                    }
                }
            }
        }
    }
}

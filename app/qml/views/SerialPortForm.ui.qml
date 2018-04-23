import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Page {
    width: 600
    height: 400
    property alias bauds: bauds
    property alias ports: ports
    property alias open: open

    title: qsTr("SerialPort")

    GridLayout {
        anchors.centerIn: parent

        columns: 2
        Label {
            text: qsTr("Port: ")
        }
        ComboBox {
            id: ports
        }

        Label {
            text: qsTr("BaudRate: ")
        }
        ComboBox {
            id: bauds
        }
        Rectangle {
            width: 3
            height: 3
            color: "transparent"
        }
        Button {
            id: open
            text: qsTr("Open")
            enabled: ports.currentText.length > 0 && bauds.currentText.length > 0
        }
    }
}

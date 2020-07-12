import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0
import QtQml 2.2

Page {
    width: 400
    height: 400
    property alias connectButton: connectButton
    property alias port: port
    property alias host: host
    property alias ebay: ebay

    title: qsTr("WiFi")

    GridLayout {
        id: layout
        anchors.centerIn: parent
        columns: 2
        Label {
            text: qsTr("Host:")
        }
        TextField {
            id: host
            text: "192.168.0.10"
            placeholderText: qsTr("192.168.0.10")
        }
        Label {
            text: qsTr("Port:")
        }
        TextField {
            id: port
            text: "35000"
            placeholderText: qsTr("35000")
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }
        Rectangle {
            width: 3
            height: 3
            color: "transparent"
        }
        Button {
            id: connectButton
            text: qsTr("Connect")
            enabled: host.length > 0 && port.length > 0
        }
    }

    Label {
        id: ebay
        anchors.top: layout.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        text: qsTr("OBDII Wifi: <a href=\"https://www.ebay.com/usr/catastefy\">ebay - catastefy</a>")
        wrapMode: Text.Wrap
    }

    Label {
        anchors.top: ebay.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        visible: Qt.platform.os == "android"
        text: qsTr("NOTE: turn off mobil data and connect to OBDII wifi.")
        wrapMode: Text.Wrap
    }

    Settings {
        property alias host: host.text
        property alias port: port.text
    }
}

import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import com.tomicooler.e46oil 1.0

ApplicationWindow {
    visible: true
    width: 1020
    height: 720
    title: qsTr("E46Oil")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            icon.name: stackView.depth > 1 ? qsTr("Back") : qsTr("Settings")
            icon.source: stackView.depth > 1 ? "qrc:/icons/navigate_before-black-48dp.svg" : "qrc:/icons/settings-black-48dp.svg"
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            id: title
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Store {
        id: store
        anchors.fill: parent
    }

    Drawer {
        id: drawer
        width: parent.width * 0.4
        height: parent.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("SerialPort")
                width: parent.width
                visible: store.serialAvailable
                onClicked: {
                    stackView.push(serialport)
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("WiFi")
                width: parent.width
                onClicked: {
                    stackView.push(wifipage)
                    drawer.close()
                }
            }
        }
    }

    Component {
        id: dashboard
        Dashboard {
            width: stackView.width
            height: stackView.height
            model: store.model
        }
    }

    Component {
        id: serialport
        SerialPort {
            serial: store.serial
            width: stackView.width
            height: stackView.height
        }
    }

    Component {
        id: wifipage
        WiFiInterface {
            wifi: store.wifi
            width: stackView.width
            height: stackView.height
        }
    }

    StackView {
        id: stackView
        initialItem: dashboard
        anchors.fill: parent
    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
        store.infoDialog.displayDialog(qsTr("Welcome to E46Oil"),
                                       qsTr("<b>E46Oil</b> uses <a href=\"http://markgardnergibson.com/BMW/protocol.html\">DS2</a> protocol " +
                                            "to get the oil temperature from the motronic ECU. The standard BMW diagnostic tools, " +
                                            "e.g: <b>INPA</b> uses the same mechanism (K-BUS & DS2).<br/><br/>" +
                                            "<b>Requirements:</b>" +
                                            "<ul>" +
                                            "<li>BMW E46 M54B30 (MS 43.0 in INPA)</li>" +
                                            "<li>K+DCAN capable WiFi OBDII diag interface - <a href=\"https://www.ebay.com/usr/catastefy\">ebay</a></li>" +
                                            "</ul><br/>" +
                                            "<b>USAGE: connect to your diagnostic adapter (Menu > WiFi)!</b><br/><br/>" +
                                            "This is an open source project - <a href=\"https://github.com/tomicooler/bmwe46oil\">GitHub</a> - feel free to contribute.<br/>" +
                                            "Credits to <a href=\"https://www.cliosport.net/threads/e36-track-day-hack-m52-m54-3-0l-engine-build.717773/page-15#post-10524146\">f0xy</a> and " +
                                            "<a href=\"https://www.bimmerforums.com/forum/showthread.php?1887229-E46-Can-bus-project\">Thaniel</a> for sharing their knowledge."
                                            ));
    }
}

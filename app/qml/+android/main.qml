import QtQuick 2.10
import QtQuick.Controls 2.3
import QuickFlux 1.1
import "./views"
import "./actions"
import "./stores"

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("BMW E46 Oil temperature (330i)")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            icon.name: stackView.depth > 1 ? qsTr("back") : qsTr("settings")
            icon.source: stackView.depth > 1 ? "qrc:/qml/icons/arrow-left.svg" : "qrc:/qml/icons/bars.svg"
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

    Drawer {
        id: drawer
        width: window.width * 0.4
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Configuration")
                width: parent.width
                onClicked: {
                    stackView.push(config)
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
            facade: MainStore.facade
            oil: MainStore.oil
            width: stackView.width
            height: stackView.height
        }
    }

    Component {
        id: wifipage
        WiFiInterface {
            wifi: MainStore.wifi
            width: stackView.width
            height: stackView.height
        }
    }

    Component {
        id: config
        Config {
            facade: MainStore.facade
            oil: MainStore.oil
            width: stackView.width
            height: stackView.height
        }
    }

    StackView {
        id: stackView
        initialItem: dashboard
        anchors.fill: parent
    }

    Common {
        anchors.fill: parent
    }
}

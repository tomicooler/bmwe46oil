import QtQuick 2.14
import Qt.labs.settings 1.0
import com.tomicooler.e46oil 1.0

Item {
    property alias serial: serial
    property alias wifi: wifi
    property alias infoDialog: infoDialog
    property bool interfaceWifi: false
    property alias model: facade.model
    property bool serialAvailable: true
    property alias facade: facade

    InfoDialog {
        id: infoDialog
        anchors.fill: parent
    }

    Facade {
        id: facade

        onSendData: {
            if (interfaceWifi) {
                wifi.sendData(data);
            } else {
                serial.sendData(data);
            }
        }
    }

    SerialInterface {
        id: serial

        onConnected: {
            interfaceWifi = false;
            facade.setDelay(40);
            facade.connected();
        }

        onDataReceived: {
            facade.dataReceived(data);
        }

        onDisplayDialog: {
            infoDialog.displayDialog(title, details);
        }
    }

    TCPInterface {
        id: wifi

        onConnected: {
            interfaceWifi = true;
            facade.setDelay(0);
            facade.connected();
        }

        onDataReceived: {
            facade.dataReceived(data);
        }

        onDisplayDialog: {
            infoDialog.displayDialog(title, details);
        }
    }
}

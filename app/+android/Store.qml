import QtQuick 2.14
import Qt.labs.settings 1.0
import com.tomicooler.e46oil 1.0

Item {
    property alias wifi: wifi
    property alias infoDialog: infoDialog
    property alias model: facade.model
    property alias serial: serial
    property bool serialAvailable: false
    property alias facade: facade

    InfoDialog {
        id: infoDialog
        anchors.fill: parent
    }

    Item {
        id: serial
    }

    Facade {
        id: facade

        onSendData: {
            wifi.sendData(data);
        }
    }

    TCPInterface {
        id: wifi

        onConnected: {
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

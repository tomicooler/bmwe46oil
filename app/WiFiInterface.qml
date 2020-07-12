import QtQuick 2.12
import com.tomicooler.e46oil 1.0

WiFiInterfaceForm {
    property TCPInterface wifi

    connectButton.enabled: !wifi.connecting
    connectButton.onClicked: {
        wifi.connectToHost(host.text, Number(port.text));
    }

    ebay.onLinkActivated: {
        Qt.openUrlExternally(link);
    }
}

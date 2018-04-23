import QtQuick 2.4
import QuickFlux 1.1
import com.tomi.bmwe46oil 1.0
import "../actions"

WiFiInterfaceForm {
    property TCPInterface wifi

    connectButton.enabled: !wifi.connecting
    connectButton.onClicked: {
        wifi.connectToHost(host.text, Number(port.text));
        AppActions.chooseWifi();
    }

    ebay.onLinkActivated: {
        Qt.openUrlExternally(link);
    }
}

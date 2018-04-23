import QtQuick 2.0
import QuickFlux 1.1
import com.tomi.bmwe46oil 1.0
import "../actions"

SerialPortForm {
    property SerialInterface serial

    ports.model: serial.portNames()
    bauds.model: serial.standardBaudRates()

    open.onClicked: {
        serial.setBaud(Number(bauds.currentText));
        serial.setPort(ports.currentText);
        serial.open();
        AppActions.chooseSerial();
    }

    Component.onCompleted: {
        ports.currentIndex = ports.find(serial.port);
        bauds.currentIndex = bauds.find(serial.baud);
    }
}

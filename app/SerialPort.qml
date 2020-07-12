import QtQuick 2.14
import com.tomicooler.e46oil 1.0

SerialPortForm {
    property SerialInterface serial

    ports.model: serial.portNames()
    bauds.model: serial.standardBaudRates()

    open.onClicked: {
        serial.setBaud(Number(bauds.currentText));
        serial.setPort(ports.currentText);
        serial.open();
    }

    Component.onCompleted: {
        ports.currentIndex = ports.find(serial.port);
        bauds.currentIndex = bauds.find(serial.baud);
    }
}

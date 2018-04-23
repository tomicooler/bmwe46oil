pragma Singleton
import QtQuick 2.0
import QtQml 2.2
import QuickFlux 1.1
import com.tomi.bmwe46oil 1.0
import "../actions"

Store {
    property alias oil: oil
    property alias facade: facade
    property alias serial: serial
    property alias wifi: wifi
    property QtObject dataInterface: serial

    bindSource: AppDispatcher

    DataExtractor {
        id: oil
        config.bytepos: 12
        config.length: 1
        config.converter.multiplier: 0.796098
        config.converter.offset: -48.0137
        data.data: 0
    }

    Facade {
        id: facade
        request.ecu: "12"
        request.data: "0b03"
        onMessageReceived: {
            oil.messageReceived(message);
        }
        onSendData: {
            dataInterface.sendData(data);
        }
    }

    SerialInterface {
        id: serial

        onDisplayDialog: {
            AppActions.displayDialog(title, details);
        }
    }

    TCPInterface {
        id: wifi

        onDisplayDialog: {
            AppActions.displayDialog(title, details);
        }
    }

    Connections {
        id: receiver
        target: dataInterface
        onDataReceived: {
            facade.dataReceived(data);
        }
    }

    Filter {
        type: ActionTypes.chooseSerial
        onDispatched: {
            dataInterface = serial;
        }
    }

    Filter {
        type: ActionTypes.chooseWifi
        onDispatched: {
            dataInterface = wifi;
        }
    }
}

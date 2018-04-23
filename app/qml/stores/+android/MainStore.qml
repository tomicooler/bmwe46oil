pragma Singleton
import QtQuick 2.0
import QuickFlux 1.1
import com.tomi.bmwe46oil 1.0
import "../actions"

Store {
    property alias oil: oil
    property alias facade: facade
    property alias wifi: wifi

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
            wifi.sendData(data);
        }
    }

    TCPInterface {
        id: wifi
        onDataReceived: {
            facade.dataReceived(data);
        }
        onDisplayDialog: {
            AppActions.displayDialog(title, details);
        }
    }
}

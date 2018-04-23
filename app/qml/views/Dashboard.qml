import QtQuick 2.0
import QtQuick.Controls.Material 2.3
import com.tomi.bmwe46oil 1.0

DashboardForm {
    property DataExtractor oil
    property Facade facade

    oilLabel.text: oil.data.data.toFixed(2)
    oilLabel.color: getLabelColor()
    thermo.source: getThermoSource()

    powerButton.onCheckedChanged: {
        if (powerButton.checked) {
            facade.start();
        } else {
            facade.stop();
        }
    }

    github.onLinkActivated: {
        Qt.openUrlExternally(link);
    }

    function getLabelColor() {
        var color;
        if (oil.data.data <= 80) {
            color = Material.color(Material.Blue);
        } else if (oil.data.data <= 120) {
            color = Material.color(Material.Green);
        } else {
            color = Material.color(Material.Red);
        }
        return color;
    }

    function getThermoSource() {
        var source = "qrc:/qml/icons/thermometer-";
        if (oil.data.data <= 40) {
            source += "empty";
        } else if (oil.data.data <= 70) {
            source += "half";
        } else if (oil.data.data <= 100) {
            source += "three-quarters";
        } else {
            source += "full";
        }
        return source + ".svg";
    }
}

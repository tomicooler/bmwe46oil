import QtQuick 2.0
import QtQuick.Controls.Material 2.3
import com.tomicooler.e46oil 1.0

DashboardForm {
    property Model model

    oilLabel.text: model.oil.toFixed(2)
    oilLabel.color: getLabelColor()
    thermo.source: getThermoSource()

    github.onLinkActivated: {
        Qt.openUrlExternally(link);
    }

    function getLabelColor() {
        var color;
        if (model.oil <= 80) {
            color = Material.color(Material.Blue);
        } else if (model.oil <= 120) {
            color = Material.color(Material.Green);
        } else {
            color = Material.color(Material.Red);
        }
        return color;
    }

    function getThermoSource() {
        var source = "qrc:/icons/thermometer-";
        if (model.oil <= 40) {
            source += "empty";
        } else if (model.oil <= 70) {
            source += "half";
        } else if (model.oil <= 100) {
            source += "three-quarters";
        } else {
            source += "full";
        }
        return source + ".svg";
    }
}

import QtQuick 2.0
import QtQml 2.11
import QtQuick.Controls.Material 2.3
import com.tomi.bmwe46oil 1.0

DashboardForm {
    property Facade facade

    property color backgroundColor: Material.color(Material.Grey, Material.Shade900)
    property color indicatorColor: Material.color(Material.Red)

    clutch.backgroundColor: backgroundColor
    clutch.indicatorColor: indicatorColor
    brake.backgroundColor: backgroundColor
    brake.indicatorColor: indicatorColor
    throttle.backgroundColor: backgroundColor
    throttle.indicatorColor: indicatorColor
    steeringWheel.backgroundColor: backgroundColor
    steeringWheel.indicatorColor: indicatorColor

    powerButton.onCheckedChanged: {
        if (powerButton.checked) {
            facade.start();
        } else {
            facade.stop();
        }
    }

    buttonDown.onCheckedChanged: {
        if (buttonDown.checked) facade.control.pressDown(); else facade.control.releaseDown();
    }

    buttonUp.onCheckedChanged: {
        if (buttonUp.checked) facade.control.pressUp(); else facade.control.releaseUp();
    }

    buttonLeft.onCheckedChanged: {
        if (buttonLeft.checked) facade.control.pressLeft(); else facade.control.releaseLeft();
    }

    buttonRight.onCheckedChanged: {
        if (buttonRight.checked) facade.control.pressRight(); else facade.control.releaseRight();
    }
}

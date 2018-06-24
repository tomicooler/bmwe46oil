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

    clutch.position: facade.clutch ? 1 : 0
    brake.position: facade.brake ? 1 : 0
    throttle.position: facade.throttle.data.data / 84.0
    steeringWheel.rotation: facade.steering.data.data * -1.0

    brake.onPositionChanged: {
        buttonDown.checked = brake.position == 1;
    }

    throttle.onPositionChanged: {
        buttonUp.checked = throttle.position > 0.2
    }

    steeringWheel.onRotationChanged: {
        console.log("ROTATION ", steeringWheel.rotation);
        buttonLeft.checked = steeringWheel.rotation < -20.0 && steeringWheel.rotation > -150;
        buttonRight.checked = steeringWheel.rotation < -200 && steeringWheel.rotation > -2860.0 && !buttonLeft.checked;
    }

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

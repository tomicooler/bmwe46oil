import QtQuick 2.4

PedalForm {
    property double position: 0.0
    positionIndicator.height: position * height
}

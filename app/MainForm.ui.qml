import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    property alias startTimeLabel: startTimeLabel
    property alias elapsedTimeLabel: elapsedTimeLabel
    property alias recordingSwitch: recordingSwitch
    property alias latencyLabel: latencyLabel
    property alias dashboard: dashboard
    width: 640
    height: 480
    title: qsTr("E46Oil")

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.margins: 10
            spacing: 10

            Item {
                Layout.fillWidth: true
            }

            Label {
                id: startTimeLabel
            }

            Label {
                id: elapsedTimeLabel
            }

            Switch {
                id: recordingSwitch
                text: qsTr("Recording")
            }

            Item {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Item {
                Layout.fillWidth: true
            }

            Label {
                id: latencyLabel
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Dashboard {
            id: dashboard
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}

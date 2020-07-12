import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0

Page {
    width: 600
    height: 400
    property alias github: github
    property alias thermo: thermo
    property alias oilLabel: oilLabel

    title: qsTr("E46Oil")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item {
                Layout.fillWidth: true
            }

            Image {
                id: thermo
                sourceSize: "32x64"
                source: "qrc:/qml/icons/thermometer-three-quarters.svg"
                antialiasing: true
                LinearGradient {
                    anchors.fill: parent
                    source: thermo
                    antialiasing: true
                    gradient: Gradient {
                        GradientStop {
                            position: 0.0
                            color: "#008AC9"
                        }
                        GradientStop {
                            position: 0.5
                            color: "#2B115A"
                        }
                        GradientStop {
                            position: 1.0
                            color: "#F11A22"
                        }
                    }
                }
            }

            Label {
                id: oilLabel
                text: "12"
                font.pixelSize: Qt.application.font.pixelSize * 3
            }
            Label {
                text: qsTr("℃")
                font.pixelSize: Qt.application.font.pixelSize * 3
                color: oilLabel.color
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Label {
            Layout.fillWidth: true
            text: qsTr("Tested with BMW E46 330i (facelifted version) - MS 43.0 for M54 INPA. Any other vehichle may not work, use it at your own risk!")
            wrapMode: Label.Wrap
            horizontalAlignment: Text.AlignHCenter
        }
        Label {
            id: github
            Layout.fillWidth: true
            text: qsTr("This is an open source project - <a href=\"https://github.com/tomicooler/bmwe46oil\">GitHub</a> - feel free to contribute.")
            wrapMode: Label.Wrap
            horizontalAlignment: Text.AlignHCenter
        }
    }
}

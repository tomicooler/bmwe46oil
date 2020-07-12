import QtQuick 2.12
import QtQuick.Controls 2.12

Item {

    function displayDialog(title, details) {
        dialog.title = title;
        dialog.details = details;
        dialog.open();
    }

    Dialog {
        id: dialog

        width: parent.width * 0.8
        height: parent.height * 0.8
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        standardButtons: Dialog.Ok
        property string details: ""

        Label {
            text: dialog.details
            anchors.fill: parent
            anchors.margins: 10
            verticalAlignment: Label.AlignVCenter
            wrapMode: Label.Wrap

            onLinkActivated: {
                Qt.openUrlExternally(link);
            }
        }
    }

}

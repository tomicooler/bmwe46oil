import QtQuick 2.10
import QtQuick.Controls 2.3
import QuickFlux 1.1
import "../actions"

Item {

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

    AppListener {
        Filter {
            type: ActionTypes.displayDialog
            onDispatched: {
                dialog.title = message.title;
                dialog.details = message.details;
                dialog.open();
            }
        }
    }

    Component.onCompleted: {
        AppActions.displayDialog(
                    qsTr("Quick guide"),
                    qsTr("<b>BMW E46 Oil</b> uses <a href=\"http://markgardnergibson.com/BMW/protocol.html\">DS2</a> protocol " +
                         "to get the oil temperature from the motronic ECU. The standard BMW diagnostic tools, " +
                         "e.g: <b>INPA</b> uses the same mechanism (K-BUS & DS2).<br/><br/>" +
                         "<b>Requirements:</b>" +
                         "<ul>" +
                         "<li>BMW E46 M54B30 (MS 43.0 in INPA)</li>" +
                         "<li>K+DCAN capable WiFi OBDII diag interface - <a href=\"https://www.ebay.com/usr/catastefy\">ebay</a></li>" +
                         "</ul><br/>" +
                         "<b>USAGE: connect to your diagnostic adapter (Menu > WiFi)!</b><br/><br/>" +
                         "This is an open source project - <a href=\"https://github.com/tomicooler/bmwe46oil\">GitHub</a> - feel free to contribute.<br/>" +
                         "Credits to <a href=\"https://www.cliosport.net/threads/e36-track-day-hack-m52-m54-3-0l-engine-build.717773/page-15#post-10524146\">f0xy</a> and " +
                         "<a href=\"https://www.bimmerforums.com/forum/showthread.php?1887229-E46-Can-bus-project\">Thaniel</a> for sharing their knowledge."
                         )
                    )
    }

}

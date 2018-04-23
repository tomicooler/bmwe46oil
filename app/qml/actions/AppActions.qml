pragma Singleton
import QtQuick 2.0
import QuickFlux 1.1
import "./"

ActionCreator {
    signal chooseWifi();
    signal chooseSerial();
    signal displayDialog(string title, string details);
}

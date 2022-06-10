import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SAL 1.0

/*
This button represent the Play and Pause button.
*/
Item {
    id: root

    property alias enabled: btn.enabled

    Button {
        id: btn
        anchors.fill: parent
        enabled: false

        text: ">"
    }
}
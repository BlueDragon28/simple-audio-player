import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
This button represent the Play and Pause button.
*/
Item {
    id: root

    property alias enabled: btn.enabled

    // This two functions change the icon inside the btn.
    function playing() {
        btn.icon.source = "qrc:///images/pause.png"
    }

    function stopping() {
        btn.icon.source = "qrc:///images/play.png"
    }

    signal clicked()

    Button {
        id: btn
        anchors.fill: parent
        enabled: false

        // By default, set the source image of the btn to play.
        icon.source: "qrc:///images/play.png"
        icon.width: parent.width - 8
        icon.height: parent.height - 8
        icon.color: enabled ? "#444" : "lightgray"

        // When clicked, redirect the signal to the root item.
        onClicked: root.clicked()
    }
}
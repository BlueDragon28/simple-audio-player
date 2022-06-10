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

    // This two functions change the text inside the btn.
    function playing() {
        btn.text = "| |"
    }

    function stopping() {
        btn.text = ">"
    }

    signal clicked()

    Button {
        id: btn
        anchors.fill: parent
        enabled: false

        text: ">"

        // When clicked, redirect the signal to the root item.
        onClicked: root.clicked()
    }
}
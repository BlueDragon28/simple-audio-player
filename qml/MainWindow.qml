import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

ApplicationWindow {
    id: root
    visibility: Window.Maximized
    visible: true

    // Menu bar at the top of the program.
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("Quit"); onTriggered: root.close(); }
        }
    }

    Content {
        anchors.fill: parent
    }
}
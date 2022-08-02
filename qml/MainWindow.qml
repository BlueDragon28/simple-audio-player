import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
The main window of the application.
The basic content (menu, toolbar and the center content are displayed).
*/
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

    // Main content of the window.
    Content {
        anchors.fill: parent
    }

    // Player control.
    footer: ToolBar {
        height: 41
        position: ToolBar.Footer

        PlayerControl {
            anchors.fill: parent
        }
    }
}

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Controls.Fusion 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/config.js" as AppConfig

/*
The main window of the application.
The basic content (menu, toolbar and the center content are displayed).
*/
ApplicationWindow {
    id: root
    visible: true

    // Menu bar at the top of the program.
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("Quit"); onTriggered: root.close(); }
        }

        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("Options"); onTriggered: SAPOptions.openOptions(); }
        }

        Menu {
            title: qsTr("Help")
            Action { text: qsTr("About"); onTriggered: SAPOptions.openAbout(); }
            Action { text: qsTr("License"); onTriggered: SAPOptions.openLicense(); }
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

    /*
    System Media Control, communicate with the system about the stream currently played
    and to receive signal when the media keys are pressed.
    */
    SystemMediaControlItem {
        // When the System Media Control ask the window to be raised.
        onRaise: {
            root.requestActivate();
        }
    }

    // Called when the item is initialized.
    Component.onCompleted: {
        const status = AppConfig.loadApplicationWindowStatus();

        // Apply informations to the application window.
        if (status.x) {
            x = status.x;
        }

        if (status.y) {
            y = status.y;
        }

        if (status.width) {
            width = status.width;
        }

        if (status.height) {
            height = status.height;
        }

        visibility = status.maximized ? Window.Maximized : Window.Windowed;

        PlaybackControlSystem.salPlayer = Player;
        PlaybackControlSystem.spotifyAPI = SpotifyAPI;
    }

    // Called when the user try to close the window.
    onClosing: function(close) {
        // Save the status of the application window.
        AppConfig.saveApplicationWindowStatus(root);
    }
}

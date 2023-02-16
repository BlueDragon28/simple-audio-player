import QtCore 6.2
import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.2
import SimpleAudioPlayer 1.0

/*
This is the playlist section, the user can create and play his favorite sounds.
*/
Item {
    id: root
    
    Rectangle {
        anchors.fill: parent
        color: "white"

        ToolBar {
            id: topBar

            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 34

            Label {
                anchors.centerIn: parent
                text: "Playlist"
            }

            Row {
                anchors.fill: parent
                anchors.margins: 4

                Button {
                    text: "Save"
                    height: parent.height
                    onClicked: saveDialog.open()
                }

                Button {
                    text: "Open"
                    height: parent.height
                    onClicked: openDialog.open()
                }
            }
        }

        PlayListContent {
            id: playListContent
            anchors.top: topBar.bottom
            anchors.left: parent.left
            width: parent.width
            height: parent.height - topBar.height
        }
    }

    SystemDialog {
        id: saveDialog
        title: "Save Playlist"
        mode: "SAVE"
        directory: StandardPaths.standardLocations(StandardPaths.MusicLocation)[0]
        defaultSuffix: "json"
        nameFilters: ["JSON (*.json)", "All Files (*)"]

        onAccepted: function(filePath) {
            playListContent.savePlaylist(filePath);
        }
    }

    SystemDialog {
        id: openDialog
        title: "Open Playlist"
        mode: "OPEN"
        directory: StandardPaths.standardLocations(StandardPaths.MusicLocation)[0]
        nameFilters: ["JSON (*.json)", "All Files (*)"]

        onAccepted: function(filePath) {
            playListContent.loadPlaylist(filePath);
        }
    }
}

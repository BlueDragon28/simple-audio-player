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

    function savePlaylist() {
        if (!playListContent.isModified) {
            return;
        }

        if (playListContent.isFromFile) {
            return playListContent.savePlaylist();
        }

        saveDialog.open();
    }

    function openPlaylist() {
        openDialog.open()
    }
    
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
                text: playListContent.playlistTitle
            }

            Row {
                anchors.fill: parent
                anchors.topMargin: 3
                anchors.bottomMargin: 3
                spacing: 4

                Button {
                    id: someButton
                    width: height
                    height: parent.height
                    icon.source: "qrc:///images/diskette.png"
                    icon.width: topBar.height
                    onClicked: root.savePlaylist()
                }

                Button {
                    text: "Open"
                    width: height
                    height: parent.height
                    icon.source: "qrc:///images/folder.png"
                    icon.width: topBar.height
                    onClicked: root.openPlaylist()
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

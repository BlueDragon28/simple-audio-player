import QtCore 6.2
import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

/*
This is the playlist section, the user can create and play his favorite sounds.
*/
Item {
    id: root

    property string checkSaveAction: ""
    property bool bypassCheckSave: false

    function engageNextAction() {
        if (checkSaveAction === "OPEN") {
            openPlaylist();
        } else if (checkSaveAction === "NEW") {
            newPlaylist();
        }

        checkSaveAction = "";
        bypassCheckSave = false;
    }

    function savePlaylist() {
        if (!playListContent.isModified) {
            return;
        }

        if (playListContent.isFromFile) {
            playListContent.savePlaylist();
            return engageNextAction();
        }

        saveDialog.open();
    }

    function isSaved(action) {
        if (!playListContent.isModified) {
            return true;
        }

        if (typeof action === "string") {
            root.checkSaveAction = action
        }

        if (documentModifiedDialog.dialogIsOpen) {
            return false;
        }
        
        documentModifiedDialog.dialogIsOpen = true;
        documentModifiedDialog.open();

        return false;
    }

    function openPlaylist() {
        if (!isSaved("OPEN") && !bypassCheckSave) {
            return;
        }

        openDialog.open();
    }

    function newPlaylist() {
        if (!isSaved("NEW") && !bypassCheckSave) {
            return;
        }

        playListContent.newPlaylist();
    }
    
    Rectangle {
        anchors.fill: parent
        color: palette.base

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
                    width: height
                    height: parent.height
                    icon.source: "qrc:///images/new-document.png"
                    icon.width: topBar.height
                    icon.color: palette.buttonText
                    onClicked: root.newPlaylist()
                }

                Button {
                    width: height
                    height: parent.height
                    icon.source: "qrc:///images/diskette.png"
                    icon.width: topBar.height
                    icon.color: palette.buttonText
                    onClicked: root.savePlaylist()
                }

                Button {
                    width: height
                    height: parent.height
                    icon.source: "qrc:///images/folder.png"
                    icon.width: topBar.height
                    icon.color: palette.buttonText
                    onClicked: root.openPlaylist()
                }

                Button {
                    width: height
                    height: parent.height
                    icon.source: "qrc:///images/down.png"
                    icon.width: topBar.height
                    icon.color: palette.buttonText
                    onClicked: playListContent.moveSelectionDown()
                }

                Button {
                    width: height
                    height: parent.height
                    icon.source: "qrc:///images/up.png"
                    icon.width: topBar.height
                    icon.color: palette.buttonText
                    onClicked: playListContent.moveSelectionUp()
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

            engageNextAction();
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

    MessageDialog {
        id: documentModifiedDialog
        text: "The playlist has beed modified.\nDo you want to save it?"
        buttons: MessageDialog.Save | MessageDialog.Discard | MessageDialog.Cancel
        /*onRejected: function() {*/
        /*}*/
        /*onDiscarded: function() {*/
        /*}*/
        onButtonClicked: function(button, role) {
            switch (role) {
            case MessageDialog.AcceptRole:
            {
                root.savePlaylist()
                dialogIsOpen = false;
            } break;
            case MessageDialog.RejectRole:
            {
                root.checkSaveAction = ""
                root.bypassCheckSave = false;
                dialogIsOpen = false;
            } break;
            case MessageDialog.DestructiveRole:
            {
                bypassCheckSave = true;
                root.engageNextAction();
                dialogIsOpen = false;
                close();
            } break;
            }
        }

        property bool dialogIsOpen: false
    }
}

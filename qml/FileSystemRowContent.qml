import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
This item display the music of the file system in one row.
The user can start a music from there.
*/
Item {
    id: root

    property alias path: fileSystemModel.path

    /*
    Move to the parent of the current directory.
    */
    function cdUp() {
        fileSystemModel.cdUp()
    }

    /*
    Move to the last opened directory (if any).
    */
    function cdDown() {
        fileSystemModel.cdDown()
    }

    ListView {
        id: fileSystemView
        anchors.fill: parent
        clip: true

        /*
        This model store the directories and files list inside a folder.
        */
        model: FileSystemModel {
            id: fileSystemModel
            path: "/"
        }

        delegate: fileSystemDelegate
        
        // Highlith the selected item by the user.
        highlight: Rectangle {
            color: "lightsteelblue"
        }
        // The time needed for the highlight to resize to the size of the delegate. 
        highlightResizeDuration: 500
        highlightMoveDuration: 150

        spacing: 5
    }

    Component {
        id: fileSystemDelegate

        Label {
            width: fileSystemView.width
            text: name

            // Mouse area to change the selection of the fileSystemView.
            MouseArea {
                anchors.fill: parent
                
                // When clicked, change the current index of the fileSystemView
                onClicked: fileSystemView.currentIndex = index

                /*
                When the user double click on an item, if its a directory, move the view inside
                this directory.
                */
                onDoubleClicked: {
                    if (isDir) {
                        fileSystemModel.cd(name)
                    }
                }
            }
        }
    }
}
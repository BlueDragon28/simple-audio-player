import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/simple-audio-library.js" as SAL

/*
This item display the music of the file system in a list.
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

    Column{
        anchors.fill: parent
        spacing: 0

        FileSystemViewHeader {
            id: viewHeader
            width: parent.width
        }

        ListView {
            id: fileSystemView
            /*Layout.fillHeight: true
            Layout.fillWidth: true*/
            width: parent.width
            height: parent.height - viewHeader.height
            clip: true

            /*
            This model store the directories and files list inside a folder.
            */
            model: FileSystemModel {
                id: fileSystemModel
            }

            delegate: fileSystemDelegate
            spacing: 0

            /*
            Context menu called when the user right click on the view.
            */
            FileSystemContextMenu {
                id: contextMenu
                onPlay: SAL.playSelectedMusic(fileSystemModel.selectedFilesList())
            }
        }
    }

    Component {
        id: fileSystemDelegate

        /*
        This rectangle is displaying the selection (if the item is selected or not).
        */
        TrackListRowBaseItem {
            width: fileSystemView.width
            contentHeight: fileName.height
            isItemSelected: isSelected
            isPlaying: Player.currentStream === absoluteFilePath // Check if the file is being played.
            
            // The name of the file name.
            Label {
                id: fileName
                width: viewHeader.nameSize-4
                text: name
            }

            // The last time the file was modified.
            Label {
                id: lastModification
                width: viewHeader.lastModifiedSize
                text: lastModified
            }

            // The file size.
            Label {
                id: fileSize
                width: viewHeader.sizeSize-4
                text: size
            }

            // When clicked, select the item.
            onClicked: function (mouse) {
                if (mouse.button === Qt.LeftButton) { // Left button click: select items.
                    if (mouse.modifiers === Qt.ShiftModifier) {
                        fileSystemModel.shiftSelectItem(index)
                    } else if (mouse.modifiers === Qt.ControlModifier) {
                        fileSystemModel.ctrlSelectItem(index)
                    } else {
                        fileSystemModel.selectAtIndex(index)
                    }
                } else if (mouse.button === Qt.RightButton) { // Right button click: open context menu.
                    contextMenu.popup()
                } else { // Middle button click: deselect all selection.
                    fileSystemModel.clearSelection()
                }

                // Make focus on the list view.
                fileSystemView.focus = true
            }

            /*
            When double click, if it's a directory, move inside, otherwise, play the list 
            with the current item selected first.
            */
            onDoubleClicked: function(mouse) {
                // If the user is pressing the control key, or double clicking with the middle or right button, don't do anything.
                if (mouse.button === Qt.MiddleButton || 
                    mouse.button === Qt.RightButton || 
                    mouse.modifiers === Qt.ControlModifier) {
                    return;
                }
                
                if (isDir) {
                    fileSystemModel.cd(name)
                } else {
                    SAL.open(completeListPath, absoluteFilePath)
                }
            }
        }
    }
}

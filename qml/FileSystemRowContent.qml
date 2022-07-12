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
                path: "/"
            }

            delegate: fileSystemDelegate
            spacing: 0
        }
    }

    Component {
        id: fileSystemDelegate

        /*
        This rectangle is displaying the selection (if the item is selected or not).
        */
        Rectangle {
            width: fileSystemView.width
            height: fileName.height+9
            color: isSelected ? "lightsteelblue" : "transparent"

            /*
            This rectangle is displaying if the file is playing.
            */
            Rectangle {
                anchors.fill: parent
                color: Player.currentStream === absoluteFilePath ? Qt.color("#A00040FF") : "transparent"
                radius: 8

                /*
                A line row displaying informations on the file.
                */
                Row {
                    width: parent.width-8
                    height: parent.height-8
                    spacing: 2
                    x: 4
                    y: 4

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
                }

                // Line at the bottom of the row.
                Rectangle {
                    anchors.top: parent.bottom
                    anchors.topMargin: -1
                    anchors.left: parent.left

                    width: parent.width
                    height: 1
                    color: "lightgray"
                }

                // Mouse area to change the selection of the fileSystemView.
                MouseArea {
                    anchors.fill: parent
                    
                    // When clicked, select the item.
                    onClicked: {
                        if (mouse.modifiers == Qt.ShiftModifier) {
                            fileSystemModel.shiftSelectItem(index)
                        } else {
                            fileSystemModel.clearSelection()
                            fileSystemModel.setIsSelected(index, true)
                        }
                    }

                    /*
                    When the user double click on an item, if its a directory, move the view inside
                    this directory. Otherwise, try to play file and all the files next to it (if any).
                    */
                    onDoubleClicked: {
                        if (isDir) {
                            fileSystemModel.cd(name)
                        } else {
                            SAL.open(listPath)
                        }
                    }
                }
            }
        }
    }
}
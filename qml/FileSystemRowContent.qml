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
            
            // Highlith the selected item by the user.
            highlight: Rectangle {
                color: "lightsteelblue"
            }
            // The time needed for the highlight to resize to the size of the delegate. 
            highlightResizeDuration: 500
            highlightMoveDuration: 150

            spacing: 0
        }
    }

    Component {
        id: fileSystemDelegate

        Rectangle {
            width: fileSystemView.width
            height: fileName.height+9
            color: Player.currentStream === absoluteFilePath ? Qt.color("#A00040FF") : "transparent"
            radius: 8

            Row {
                width: parent.width-8
                height: parent.height-8
                spacing: 4
                x: 4
                y: 4

                // The name of the file name.
                Label {
                    id: fileName
                    width: viewHeader.nameSize-4
                    text: name
                }

                // The file size.
                Label {
                    id: fileSize
                    width: viewHeader.sizeSize-4
                    height: parent.height
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
                
                // When clicked, change the current index of the fileSystemView
                onClicked: fileSystemView.currentIndex = index

                /*
                When the user double click on an item, if its a directory, move the view inside
                this directory.
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
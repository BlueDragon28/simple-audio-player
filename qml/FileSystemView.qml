import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
This view is displaying the audio file inside the file system
of the OS. The user can navigate inside it.
*/
Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "white"

        // This toolbar hold the button and the folder location to navigate inside the file system.
        ToolBar {
            id: topBar

            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: rowLayout.height + 8

            RowLayout {
                id: rowLayout
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                anchors.topMargin: 4
                anchors.bottomMargin: 4

                // Previous folder.
                Button {
                    id: prevBtn
                    Layout.fillWidth: false
                    Layout.preferredWidth: height
                    text: "<"
                }

                // Return back to the last next folder if possible.
                Button {
                    id: nextBtn
                    Layout.fillWidth: false
                    Layout.preferredWidth: height
                    text: ">"
                }

                FolderPathLineEdit {
                    id: folderPath
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                // Go to the folder.
                Button {
                    id: searchBtn
                    Layout.fillWidth: false
                    Layout.preferredWidth: height
                    text: "s"
                }
            }
        }

        // The content of the file system is displayed here.
        FileSystemRowContent {
            id: fileSystemContent
            anchors.top: topBar.bottom
            anchors.left: parent.left
            width: parent.width
            height: parent.height - topBar.height
        }
    }
}
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
        color: palette.base

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
                height: 26

                // Previous folder.
                Button {
                    id: prevBtn
                    Layout.preferredHeight: parent.height
                    Layout.fillWidth: false
                    Layout.preferredWidth: height
                    icon.source: "qrc:///images/back.png"
                    icon.color: palette.buttonText

                    // When clicked, move the FileSystemRowContent to the parent directory.
                    onClicked: fileSystemContent.cdUp()
                }

                // Return back to the last next folder if possible.
                Button {
                    id: nextBtn
                    Layout.preferredHeight: parent.height
                    Layout.fillWidth: false
                    Layout.preferredWidth: height
                    icon.source: "qrc:///images/next.png"
                    icon.color: palette.buttonText

                    // When clicked, move the FileSystemRowContent to the last opened directory (if any).
                    onClicked: fileSystemContent.cdDown()
                }

                FolderPathLineEdit {
                    id: folderPath
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    /*
                    The path displayed is synchronized with the path of fileSystemContent.
                    Every time the path of folderPath is changing, the path of fileSystemContent is changed too.
                    */
                    text: fileSystemContent.path
                    onAccepted: fileSystemContent.updatePath()
                }

                // Go to the folder.
                Button {
                    id: searchBtn
                    Layout.preferredHeight: parent.height
                    Layout.fillWidth: false
                    Layout.preferredWidth: height
                    icon.source: "qrc:///images/magnifying-glass.png"
                    icon.color: palette.buttonText

                    onClicked: fileSystemContent.updatePath()
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

            // This function update the path of the fileSystemContent.
            function updatePath() {
                path = folderPath.text
            }
        }
    }
}
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
        spacing: 5
    }

    Component {
        id: fileSystemDelegate

        Label {
            text: name
        }
    }
}
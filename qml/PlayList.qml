import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
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
        }

        Label {
            anchors.centerIn: parent
            text: "PlayList Section"
        }
    }
}
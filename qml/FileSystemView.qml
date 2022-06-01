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

        Label {
            anchors.centerIn: parent
            text: "File System"
        }
    }
}
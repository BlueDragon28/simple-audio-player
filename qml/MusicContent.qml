import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
This view is displaying the music available on the filesystem
through a list of albums.
*/
Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "white"

        AlbumListView {
            anchors.fill: parent
        }
    }
}

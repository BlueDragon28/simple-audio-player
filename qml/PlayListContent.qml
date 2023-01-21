import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
Displaying the content of the current playlist.
*/
Item {
    id: root

    ListView {
        id: playlistContent
        anchors.fill: parent

        clip: true
        boundsBehavior: Flickable.StopAtBounds

        spacing: 0

        model: PlaylistModel {
            id: playlistContentModel
        }
    }
}
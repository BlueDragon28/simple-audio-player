import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
Displaying the content of the current playlist.
*/
Item {
    id: root

    function addItem(filesPath) {
        playlistContentModel.add(filesPath);
    }

    ListView {
        id: playlistContent
        anchors.fill: parent

        clip: true
        boundsBehavior: Flickable.StopAtBounds

        spacing: 0

        delegate: playlistDelegate

        model: PlaylistModel {
            id: playlistContentModel
        }
    }

    Component {
        id: playlistDelegate

        TrackListRowBaseItem {
            width: playlistContent.width
            contentHeight: trackName.height
            isItemSelected: isSelected
            isPlaying: Player.currentStream === filepath

            // Track Number
            Label {
                id: trackName
                width: 150
                clip: true
                text: name
            }

            // Track name
            Label {
                id: trackArtists
                width: 50
                clip: true
                text: artists
            }
        }
    }
}

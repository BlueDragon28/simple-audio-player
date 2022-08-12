import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
This item display the tracks of an album.
*/
Rectangle {
    id: root

    property alias albumName: trackListModel.album

    // View diplaying the track list.
    ListView {
        id: trackListView
        anchors.fill: parent
        clip: true
        spacing: 0

        // Tracks list model.
        model: AlbumTracksListModel {
            id: trackListModel
        }

        delegate: trackListDelegate
    }

    // Delegate of the trackListView.
    Component {
        id: trackListDelegate

        Item {
            width: trackListView.width
            height: nameOfTrack.height

            Label {
                id: nameOfTrack
                width: parent.width
                text: trackName
            }
        }
    }
}

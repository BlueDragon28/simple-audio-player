import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

// Simple previous/next button to change playlist page
Item {
    id: root

    Row {
        anchors.fill: parent
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom

            Button {
                flat: true
                enabled: SpotifyAPI.userPlaylist.hasPrevious
                text: "previous"

                onClicked: {
                    SpotifyAPI.userPlaylist.fetchPrevious();
                }
            }

            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: SpotifyAPI.userPlaylist.pageNumber + " / " + SpotifyAPI.userPlaylist.totalPages
            }

            Button {
                flat: true
                enabled: SpotifyAPI.userPlaylist.hasNext
                text: "next"

                onClicked: {
                    SpotifyAPI.userPlaylist.fetchNext();
                }
            }
        }
    }
}
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

        // Stack layout to change between album view and albumContent view.
        StackLayout {
            id: stackLayout
            anchors.fill: parent

            AlbumListView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                // When emit, move to the album content list view and display the track list of the album.
                onEnterAlbum: function(albumName) {
                    albumContent.albumName = albumName
                    stackLayout.currentIndex = 1
                }
            }

            AlbumContentListView {
                id: albumContent
                Layout.fillWidth: true
                Layout.fillHeight: true

                onBack: stackLayout.currentIndex = 0 // Set the stack layout to the albums list item.
            }

        }
    }
}

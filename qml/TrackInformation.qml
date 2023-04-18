import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/simple-audio-library.js" as SAL

/*
Displaying informations about the current track.
*/
Item {
    id: root
    implicitHeight: rect.height

    Rectangle {
        id: rect
        width: parent.width
        height: horizontalLayout.height
        color: palette.alternateBase

        ColumnLayout {
            id: horizontalLayout
            width: parent.width

            // Display album cover 
            Image {
                id: albumCover
                source: "image://coverArt/empty"
                asynchronous: true // Loading the cover image in another thread.
                fillMode: Image.PreserveAspectFit
                mipmap: true
                smooth: true
                Layout.fillWidth: parent
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: Layout.preferredWidth * (sourceSize.height / sourceSize.width)
            }

            // Displaying track informations in labels that slide when the text go beyond the boundaries.
            // Track title
            TrackLabel {
                id: trackTitle
                Layout.fillWidth: true
                Layout.leftMargin: 8
                Layout.topMargin: 8
                Layout.rightMargin: 8
            }

            TrackLabel {
                id: trackAlbum
                Layout.fillWidth: true
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                fontSize: 10
                // color: "#505050"
                secondaryInfo: true
            }

            TrackLabel {
                id: trackArtist
                Layout.fillWidth: true
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.bottomMargin: 8
                fontSize: 10
                // color: "#505050"
                secondaryInfo: true
            }
        }
    }

    // Connect to the TrackTag singleton to receive notification when tags are changed.
    Connections {
        target: TrackTag

        function onTitleChanged() {
            /*
                Display the title of the track.
                If the track doesn't have a title, use the file name.
            */
            let title = TrackTag.title
            let filePath = TrackTag.filePath

            if (title.length > 0) {
                trackTitle.text = title
            } else if (filePath.length === 0) {
                trackTitle.text = ""
            } else {
                trackTitle.text = SAL.getFileName(filePath)
            }
        }

        function onAlbumChanged() {
            /*
                Display the name of the album.
                If there is no album name, use "Unknown" instead.
            */
            let album = TrackTag.album
            let filePath = TrackTag.filePath

            if (album.length > 0) {
                trackAlbum.text = album
            } else if (filePath.length === 0) {
                trackAlbum.text = ""
            } else {
                trackAlbum.text = "Unknown"
            }
        }

        function onArtistChanged() {
            /*
                Display the artist name.
                If there is no artist name, use "Unknown" instead.
            */
            let artist = TrackTag.artist
            let filePath = TrackTag.filePath

            if (artist.length > 0) {
                trackArtist.text = artist
            } else if (filePath.length === 0) {
                trackArtist.text = ""
            } else {
                trackArtist.text = "Unknown"
            }
        }

        function onCoverArtChanged() {
            /*
            Display the album cover (if any).
            */
            let albumName = TrackTag.album
            albumCover.source = "image://coverArt/" + albumName
        }

        function onCoverArtIsEmpty() {
            /*
            Display default image.
            */
            albumCover.source = "image://coverArt/empty"
        }
    }
}

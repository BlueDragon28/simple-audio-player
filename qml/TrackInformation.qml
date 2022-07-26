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
        color: "lightgray"

        ColumnLayout {
            id: horizontalLayout
            width: parent.width

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
                color: "#505050"
            }

            TrackLabel {
                id: trackArtist
                Layout.fillWidth: true
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.bottomMargin: 8
                fontSize: 10
                color: "#505050"
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
            } else if (filePath.length == 0) {
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
            } else if (filePath.length == 0) {
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
            } else if (filePath.length == 0) {
                trackArtist.text = ""
            } else {
                trackArtist.text = "Unknown"
            }
        }
    }
}
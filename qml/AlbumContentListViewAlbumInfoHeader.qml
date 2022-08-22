import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
This element display the informations about an album.
*/
Item {
    id: root
    height: 250 // Default height of the album header element.

    property string albumName: ""
    signal back() // Go back to the albums list.

    // When albumName change, change album info.
    onAlbumNameChanged: {
        coverImg.source = "image://coverArt/" + albumName // Change album cover art.
    }

    Column {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        // Button to allow to go back to the albums list.
        Button {
            id: goBack
            text: "<"
            width: height

            onClicked: root.back()
        }

        // Image displaying the album cover art.
        Image {
            id: coverImg
            height: root.height - goBack.height - 8
            width: height
            asynchronous: true
        }
    }
}

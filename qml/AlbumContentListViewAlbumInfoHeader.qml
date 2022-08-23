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
        albumNameLabel.text = albumName
    }

    // Main layout for the album info header.
    Column {
        anchors.fill: parent
        spacing: 0

        // Toolbar
        ToolBar {
            id: toolBar
            width: parent.width
            height: toolBarLayout.height + 8

            RowLayout {
                id: toolBarLayout
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 4
                anchors.bottomMargin: 4
                width: parent.width

                // Button to allow to go back to the albums list.
                Button {
                    id: goBack
                    text: "<"
                    Layout.preferredWidth: parent.height

                    onClicked: root.back()
                }

                // Label displaying "Album Content" text.
                Label {
                    Layout.preferredWidth: parent.width - goBack.implicitWidth
                    Layout.preferredHeight: parent.height
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: "Album Content"
                }

            }
        }

        // Informations about the album (cover art, name, etc).
        Row {
            width: parent.width
            height: parent.height - toolBar.height
            padding: 8
            spacing: 8

            // Image displaying the album cover art.
            Image {
                id: coverImg
                height: parent.height - (parent.padding * 2)
                width: height
                asynchronous: true
            }

            // Container for the album name.
            Item {
                width: parent.width - coverImg.width
                height: parent.height - (parent.padding * 2)

                // Album name
                Label {
                    id: albumNameLabel
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.bottomMargin: 8
                    font.pointSize: 24
                    font.weight: Font.Bold
                    text: "test"
                }
            }
        }
    }

    // Adding a bottom line to the header.
    Rectangle {
        width: parent.width
        height: 1
        anchors.bottom: parent.bottom
        color: "lightgray"
    }
}

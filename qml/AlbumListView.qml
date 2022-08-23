import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
Display a list of albums with the image cover and the name.
*/
Item {
    id: root

    signal enterAlbum(string albumName)

    Column {
        anchors.fill: parent

        // Toolbar displaying "Album Collection".
        ToolBar {
            id: toolbar
            width: parent.width
            height: 34 // Approximation of the height of all the toolbar in this app.

            Label {
                width: parent.width
                height: parent.height
                text: "Album Collection"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Column {
            width: parent.width
            height: parent.height - toolbar.height
            padding: 8
            clip: true

            // View displaying the albums in row and columns.
            GridView {
                id: albumView
                width: parent.width
                height: parent.height
                cellWidth: 200
                cellHeight: 250

                model: AlbumCollectionModel {
                    id: albumModel
                }

                delegate: albumDelegate

                // Rectangle displaying the current selection.
                highlight: Rectangle {
                    color: "lightsteelblue"
                }
                highlightMoveDuration: 0
            }

            // Delegate of the GridView to display the album cover and the album name.
            Component {
                id: albumDelegate

                // Root item of the delegate with the size of a albumView cell.
                Item {
                    width: albumView.cellWidth
                    height: albumView.cellHeight

                    // Display the the cover and the name under it.
                    Column {
                        anchors.fill: parent
                        anchors.topMargin: 10
                        spacing: 3

                        Image {
                            id: cover
                            source: "image://coverArt/" + name
                            asynchronous: true
                            fillMode: Image.PreserveAspectFit
                            width: parent.width - 20
                            height: width
                            x: 10
                            y: 10
                        }

                        Label {
                            width: parent.width
                            height: parent.height - cover.height - 3 // Height of the parent minus the height of the cover and the spacing.
                            clip: true

                            text: name
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignTop
                        }
                    }

                    // Mouse area updating the current index when clicked.
                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            albumView.currentIndex = index // Update the selection index.
                        }

                        onDoubleClicked: {
                            root.enterAlbum(name) // Notify to list the content of the album.
                        }
                    }
                }
            }
        }
    }
}

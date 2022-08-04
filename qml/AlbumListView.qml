import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
Display a list of albums with the image cover and the name.
*/
Item {
    id: root

    // View displaying the albums in row and columns.
    GridView {
        id: albumView
        anchors.fill: parent
        anchors.margins: 8
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

                Rectangle {
                    id: cover
                    width: parent.width - 20
                    height: width
                    x: 10
                    y: 10
                    color: "gray"
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

                onClicked: albumView.currentIndex = index
            }
        }
    }
}

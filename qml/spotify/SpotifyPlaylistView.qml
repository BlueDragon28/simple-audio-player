import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Item {
    id: root

    property alias model: playlistsView.model

    GridView {
        id: playlistsView
        anchors.fill: parent
        cellWidth: 200
        cellHeight: 250
        boundsBehavior: Flickable.StopAtBounds

        delegate: playlistDelegate

        highlight: Rectangle {
            color: palette.highlight
        }
        highlightMoveDuration: 0
    }

    Component {
        id: playlistDelegate

        Item {
            width: playlistsView.cellWidth
            height: playlistsView.cellHeight

            Column {
                anchors.fill: parent
                anchors.topMargin: 10
                spacing: 3

                Image {
                    id: cover
                    source: imageUrl
                    asynchronous: true
                    fillMode: Image.PreserveAspectFit
                    width: parent.width - 20
                    height: width
                    mipmap: true
                    smooth: true
                    x: 10
                    y: 10
                }

                Label {
                    width: parent.width
                    height: parent.height - cover.height - 3
                    clip: true

                    text: title
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    playlistsView.currentIndex = index
                }
            }
        }
    }
}
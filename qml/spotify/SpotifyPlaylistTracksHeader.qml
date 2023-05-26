import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Item {
    id: root

    height: 25
    implicitHeight: 25

    ToolBar {
        anchors.fill: parent
        horizontalPadding: 0

        SplitView {
            anchors.fill: parent
            orientation: Qt.horizontal

            Item {
                id: trackNumber
                
                SplitView.minimumWidth: 30
                SplitView.maximumWidth: 100

                Label {
                    anchors.fill: parent
                    text: "#"
                    clip: true 
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }

            Item {
                id: trackName

                SplitView.minimumWidth: 120
                SplitView.fillWidth: true

                Label {
                    anchors.fill: parent
                    text: "name"
                    clip: true
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }

            Item {
                id: trackArtists

                SplitView.minimumWidth: 120
                SplitView.maximumWidth: 300

                Label {
                    anchors.fill: parent
                    text: "artists"
                    clip: true
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }

            Item {
                id: trackAlbum

                SplitView.minimumWidth: 120
                SplitView.maximumWidth: 300

                Label {
                    anchors.fill: parent
                    text: "album"
                    clip: true
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }
        }
    }
}

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

import "../"

Item {
    id: root

    property QtObject tracksModel

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        SpotifyPlaylistTracksHeader {
            id: tracksHeader
            Layout.fillWidth: true
        }

        ListView {
            id: tracksListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 0
            boundsBehavior: Flickable.StopAtBounds

            model: tracksModel
            delegate: trackListDelegate
        }
    }

    Component {
        id: trackListDelegate

        TrackListRowBaseItem {
            width: tracksListView.width
            contentHeight: trackName.height
            isItemSelected: isSelected
            isPlaying: trackUri === PlaybackControlSystem.currentStream

            Label {
                id: trackNumber 
                width: tracksHeader.trackNumberWidth
                clip: true
                text: trackIndex
            }

            Label {
                id: trackName
                width: tracksHeader.trackNameWidth
                clip: true
                text: name
            }

            Label {
                id: trackArtists
                width: tracksHeader.trackArtistsWidth
                clip: true
                text: artists
            }

            Label {
                id: trackAlbum
                width: tracksHeader.trackAlbumWidth
                clip: true
                text: album
            }

            onClicked: function(mouse) {
                if (mouse.button === Qt.LeftButton) {
                    if (mouse.modifiers === Qt.ShiftModifier) {
                        tracksModel.shiftSelectItem(index);
                    } else if (mouse.modifiers == Qt.ControlModifier) {
                        tracksModel.ctrlSelectItem(index);
                    } else {
                        tracksModel.selectAtIndex(index);
                    }
                } else if (mouse.button !== Qt.RightButton) {
                    tracksModel.clearSelection();
                }

                tracksListView.focus = true;
            }

            onDoubleClicked: function(mouse) {
                if (mouse.button === Qt.MiddleButton ||
                        mouse.button === Qt.RightButton ||
                        mouse.modifiers === Qt.ControlModifier) {
                    return;
                }

                const params = {
                    contex: tracksModel.uri,
                    offset: index
                };
                SpotifyAPI.player.play(params);
            }
        }
    }
}

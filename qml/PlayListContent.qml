import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/simple-audio-library.js" as SAL

/*
Displaying the content of the current playlist.
*/
Item {
    id: root

    function addItem(filesPath) {
        playlistContentModel.add(filesPath);
    }

    PlayListContextMenu {
        id: contextMenu
    }

    ListView {
        id: playlistContent
        anchors.fill: parent

        clip: true
        boundsBehavior: Flickable.StopAtBounds

        spacing: 0

        delegate: playlistDelegate

        model: PlaylistModel {
            id: playlistContentModel
        }
    }

    Component {
        id: playlistDelegate

        TrackListRowBaseItem {
            width: playlistContent.width
            contentHeight: trackName.height
            isItemSelected: isSelected
            isPlaying: Player.currentStream === filepath

            // Track Number
            Label {
                id: trackName
                width: 150
                clip: true
                text: name
            }

            // Track name
            Label {
                id: trackArtists
                width: 50
                clip: true
                text: artists
            }

            // When clicked, select the item.
            onClicked: function (mouse) {
                if (mouse.button === Qt.LeftButton) { // Left button click: select items.
                    if (mouse.modifiers === Qt.ShiftModifier) {
                        playlistContentModel.shiftSelectItem(index)
                    } else if (mouse.modifiers === Qt.ControlModifier) {
                        playlistContentModel.ctrlSelectItem(index)
                    } else {
                        playlistContentModel.selectAtIndex(index)
                    }
                } else if (mouse.button === Qt.RightButton) { // Right button click: open context menu.
                     contextMenu.popup()
                } else { // Middle button click: deselect all selection.
                    playlistContentModel.clearSelection()
                }

                // Make focus on the list view.
                playlistContent.focus = true
            }

            // When double click, play the list with the current item selected first.
            onDoubleClicked: function(mouse) {
                // If the user is pressing the control key, or double clicking with the middle or right button, don't do anything.
                if (mouse.button === Qt.MiddleButton ||
                    mouse.button === Qt.RightButton ||
                    mouse.modifiers === Qt.ControlModifier) {
                    return;
                }

                SAL.open(playlistContentModel.pathList, filepath)
            }
        }
    }

    Connections {
        target: AddToPlaylistInterface

        function onTracksAdded(tracksPath) {
            playlistContentModel.add(tracksPath);   
        }
    }
}

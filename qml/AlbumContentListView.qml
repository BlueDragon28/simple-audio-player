import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/simple-audio-library.js" as SAL

/*
This item display the tracks of an album.
*/
Rectangle {
    id: root

    property alias albumName: trackListModel.album

    signal back() // Go back to the albums list.

    // When the album name is changed: update album info in AlbumContentListViewAlbumInfoHeader.
    onAlbumNameChanged: {
        albumInfoHeader.albumName = albumName
    }

    Column {
        anchors.fill: parent

        AlbumContentListViewAlbumInfoHeader {
            id: albumInfoHeader
            width: parent.width

            onBack: root.back() // When the back signal of the album info header is triggered, move back to the albums list.
        }

        // The header of the album list. It displaying the name of the columns and allow resizing them.
        AlbumContentListViewHeader {
            id: viewHeader
            width: parent.width
        }

        // View diplaying the track list.
        ListView {
            id: trackListView
            width: parent.width
            height: parent.height - (viewHeader.height + albumInfoHeader.height)
            clip: true
            spacing: 0

            // Tracks list model.
            model: AlbumTracksListModel {
                id: trackListModel
            }

            delegate: trackListDelegate
        }

        // Delegate of the trackListView.
        Component {
            id: trackListDelegate

            // Helper item to display the tracks.
            TrackListRowBaseItem {
                width: trackListView.width
                contentHeight: nameOfTrack.height
                isItemSelected: isSelected
                isPlaying: Player.currentStream === filePath

                // Track name
                Label {
                    id: nameOfTrack
                    width: viewHeader.nameWidth
                    clip: true
                    text: trackName
                }

                // Artists list.
                Label {
                    id: artistNames
                    width: viewHeader.artistWidth
                    clip: true
                    text: artists
                }

                // When clicked, select the item.
                onClicked: function (mouse) {
                    if (mouse.button === Qt.LeftButton) { // Left button click: select items.
                        if (mouse.modifiers === Qt.ShiftModifier) {
                            trackListModel.shiftSelectItem(index)
                        } else if (mouse.modifiers === Qt.ControlModifier) {
                            trackListModel.ctrlSelectItem(index)
                        } else {
                            trackListModel.selectAtIndex(index)
                        }
                    } else if (mouse.button === Qt.RightButton) { // Right button click: open context menu.
                        // contextMenu.popup()
                    } else { // Middle button click: deselect all selection.
                        trackListModel.clearSelection()
                    }

                    // Make focus on the list view.
                    trackListView.focus = true
                }

                // When double click, play the list with the current item selected first.
                onDoubleClicked: function(mouse) {
                    // If the user is pressing the control key, or double clicking with the middle or right button, don't do anything.
                    if (mouse.button === Qt.MiddleButton ||
                        mouse.button === Qt.RightButton ||
                        mouse.modifiers === Qt.ControlModifier) {
                        return;
                    }

                    // Read the album, starting from the current item.
                    SAL.open(trackListModel.pathList, filePath)
                }
            }
        }
    }
}

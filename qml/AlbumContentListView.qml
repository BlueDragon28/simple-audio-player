import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
This item display the tracks of an album.
*/
Rectangle {
    id: root

    property alias albumName: trackListModel.album

    Column {
        anchors.fill: parent

        // The header of the album list. It displaying the name of the columns and allow resizing them.
        AlbumContentListViewHeader {
            id: viewHeader
            width: parent.width
        }

        // View diplaying the track list.
        ListView {
            id: trackListView
            width: parent.width
            height: parent.height - viewHeader.height
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
            }
        }
    }
}

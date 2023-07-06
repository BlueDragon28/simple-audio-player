import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Item {
    id: root
    
    signal viewPlaylist()

    SpotifyPlaylistView {
        anchors.fill: parent

        model: SpotifyUserPlaylistsListModel {
            id: playlistsListModel
            playlistsAPI: SpotifyAPI.userPlaylist
        }

        onViewPlaylist: function(index) {
            root.viewPlaylist();
            SpotifyAPI.displayPlaylistDetails(playlistsListModel.get(index).href);
        }
    }
}
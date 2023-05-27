import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Rectangle {
    id: root
    color: palette.base

    states: [
        State {
            name: ""
            PropertyChanges { target: spotifySectionStackLayout; currentIndex: 0; }
        },
        State {
            name: "userProfile"
            PropertyChanges { target: spotifySectionStackLayout; currentIndex: 1; }
        },
        State {
            name: "playlistDetails"
            PropertyChanges { target: spotifySectionStackLayout; currentIndex: 2; }
        }
    ]

    StackLayout {
        id: spotifySectionStackLayout
        anchors.fill: parent
        currentIndex: 0

        SpotifyLoginScreen {
            id: spotifyLoginScreen
        }

        SpotifyUserProfile {
            id: spotifyUserProfile

            onViewPlaylist: {
                root.state = "playlistDetails";
            }
        }

        SpotifyPlaylistDetails {
            id: playlistDetail
            
            onBack: {
                root.state = "userProfile";
            }
        }
    }
    
    Connections {
        target: SpotifyAPI

        function onAuthenticated() {
            root.state = "userProfile";
        }

        function onLoggedout() {
            root.state = "";
            spotifyLoginScreen.clearField();
        }
    }

    Component.onCompleted: function() {
        // Try to restore credential if available
        SpotifyAPI.restoreCredential();
    }
}

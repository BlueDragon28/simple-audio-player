import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Rectangle {
    id: root
    color: palette.base

    StackLayout {
        id: spotifySectionStackLayout
        anchors.fill: parent
        currentIndex: 1

        SpotifyLoginScreen {
            id: spotifyLoginScreen
        }

        SpotifyUserProfile {
            id: spotifyUserProfile
        }
    }
    
}
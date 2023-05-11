import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        SpotifyPlaylistDetailsToolbar {
            Layout.fillWidth: true
        }

        SpotifyPlaylistDetailsHeader {
            Layout.fillWidth: true
            Layout.preferredHeight: 158
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            color: palette.base

            // SpotifyPlaylistDetailsTracks {
            //     anchors.fill: parent
            // }
        }
    }
}
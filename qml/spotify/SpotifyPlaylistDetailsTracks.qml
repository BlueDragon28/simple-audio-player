import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Item {
    id: root

    property QtObject tracksModel

    ColumnLayout {
        anchors.fill: parent

        SpotifyPlaylistTracksHeader {
            id: tracksHeader
            Layout.fillWidth: true
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}

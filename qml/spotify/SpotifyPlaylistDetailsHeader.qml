import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Rectangle {
    id: root
    color: palette.alternateBase

    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        Image {
            id: playlistCover
            Layout.preferredWidth: 128
            Layout.preferredHeight: 128
            sourceSize.width: Layout.preferredWidth
            sourceSize.height: Layout.preferredHeight
            source: "qrc:///images/musiqueIcon.png"
            smooth: true
            mipmap: true
        }

        ColumnLayout {
            Layout.fillHeight: true
            spacing: 0

            Item {
                Layout.fillHeight: true
            }

            Label {
                id: playlistName
                text: "playlistName"
                font.pointSize: 30
            }

            Label {
                text: "Author: abc..., Duration: 2s"
                font.pointSize: 8
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
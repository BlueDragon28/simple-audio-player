import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Rectangle {
    id: root
    color: palette.alternateBase

    property url href
    property string name
    property string authors
    property string duration

    function concartAuthorsAndDuration(authors, duration) {
        const displayedAuthors = 
            authors?.length > 0 ? authors : "...";
        const displayedDuration =
            duration?.length > 0 ? duration : "0ms";

        return `Authors: ${displayedAuthors}, Duration: ${displayedDuration}`;
    }

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
            source: href.toString().length > 0 ? 
                href :
                "qrc:///images/musiqueIcon.png"
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
                text: name.length > 0 ? name : "Unknown Playlist"
                font.pointSize: 30
            }

            Label {
                text: concartAuthorsAndDuration(authors, duration)
                font.pointSize: 8
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
This view is displaying the music available on the filesystem
through a list of artists and albums.
*/
Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "white"

//        Label {
//            anchors.centerIn: parent
//            text: "Music"
//        }

        ListView {
            anchors.fill: parent
            anchors.margins: 8

            model: AlbumCollectionModel {
                id: albumCollectionList
            }

            delegate: Label {
                text: name
            }
        }
    }
}

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "white"

        Label {
            anchors.centerIn: parent
            text: "Music"
        }
    }
}
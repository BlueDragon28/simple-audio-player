import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
Displaying informations about the current track.
*/
Item {
    id: root
    implicitHeight: rect.height

    // Alias to change the name of the track.
    property alias name: trackName.text

    Rectangle {
        id: rect
        width: parent.width
        height: horizontalLayout.height
        color: "lightgray"

        ColumnLayout {
            id: horizontalLayout
            width: parent.width

            Label {
                id: trackName
                clip: true
                verticalAlignment: Qt.AlignVCenter

                Layout.margins: 8
                Layout.fillWidth: true
            }
        }
    }
}
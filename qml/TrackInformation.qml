import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
Displaying informations about the current track inside the Player Control toolBar.
*/
Item {
    id: root
    implicitHeight: horizontalLayout.height

    // Alias to change the name of the track.
    property alias name: trackName.text

    ColumnLayout {
        id: horizontalLayout
        width: parent.width

        Label {
            id: trackName
            verticalAlignment: Qt.AlignVCenter

            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
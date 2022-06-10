import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SAL 1.0

// This is where the player control (play, pause, etc) and the volume control is placed.
Item {
    id: root

    RowLayout {
        anchors.fill: parent

        // Play pause button.
        PlayPauseButton {
            id: playBtn
            
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight
        }
    }

    // Connect to the Player singleton object and listening to the signals.
    Connections {
        target: Player

        function onIsReadyChanged(isReady) {
            playBtn.enabled = isReady
        }
    }
}
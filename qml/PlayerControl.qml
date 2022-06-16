import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SAL 1.0
import "qrc:///js/simple-audio-library.js" as SAL

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

            // Play or pause when the button is clicked.
            onClicked: SAL.playPause()
        }

        // Label showing the position time of the stream.
        Label {
            id: position

            Layout.fillWidth: false
            Layout.fillHeight: true

            verticalAlignment: Qt.AlignVCenter

            text: "00:00"
        }

        // Stream position slider
        StreamSlider {
            id: streamSlider

            Layout.fillWidth: true
            Layout.fillHeight: true

            // When the slider is moved by the user, seek the new position in the stream.
            onMoved: {
                Player.seek(value)
            }
        }

        Label {
            id: duration

            Layout.fillWidth: false
            Layout.fillHeight: true

            verticalAlignment: Qt.AlignVCenter

            text: "00:00"
        }
    }

    // Connect to the Player singleton object and listening to the signals.
    Connections {
        target: Player

        function onIsReadyChanged(isReady) {
            playBtn.enabled = isReady
            streamSlider.enabled = isReady
        }

        function onStreamPlaying() {
            playBtn.playing()
        }

        function onStreamPaused() {
            playBtn.stopping()
        }

        function onStreamStopping() {
            playBtn.stopping()
        }

        function onStartNewFile(filePath) {
            streamSlider.to = Player.streamSize()
            duration.text = SAL.parseTime(Player.streamSize(Player.SECONDS))
        }

        function onStreamPosChangeInFrames(streamPos) {
            streamSlider.value = streamPos
        }

        function onStreamPosChangeInSeconds(streamPos) {
            position.text = SAL.parseTime(streamPos)
        }
    }
}
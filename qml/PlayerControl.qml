import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/simple-audio-library.js" as SAL

// This is where the player control (play, pause, etc) and the volume control is placed.
Item {
    id: root

    FontMetrics {
        id: timeMetrics
    }

    RowLayout {
        anchors.fill: parent

        Button {
            id: previousBtn

            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight

            text: "|<"
            enabled: false

            onClicked: SAL.previous()
        }

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

        // Stop button
        Button {
            id: stopBtn

            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight

            text: "s"
            enabled: false

            // Stop the stream.
            onClicked: SAL.stop()
        }

        // Next button
        Button {
            id: nextBtn

            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight

            text: ">|"
            enabled: false

            onClicked: SAL.next()
        }

        // Label showing the position time of the stream.
        Label {
            id: position

            Layout.fillWidth: false
            Layout.fillHeight: true

            verticalAlignment: Qt.AlignVCenter

            text: "00:00"

            // Set the fixed size of the label based on the number of caracters.
            Layout.preferredWidth: timeMetrics.averageCharacterWidth * text.length
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

            // Set the fixed size of the label based on the number of caracters.
            Layout.preferredWidth: timeMetrics.averageCharacterWidth * text.length
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
            previousBtn.enabled = true
            nextBtn.enabled = true
            stopBtn.enabled = true
        }

        function onStreamPaused() {
            playBtn.stopping()
        }

        function onStreamStopping() {
            playBtn.stopping()
            previousBtn.enabled = false
            nextBtn.enabled = false
            stopBtn.enabled = false
        }

        function onStartNewFile(filePath) {
            streamSlider.to = Player.streamSize()
            duration.text = SAL.parseTime(Player.streamSize(Player.SECONDS))
            PlayingList.next(filePath)
        }

        function onEndFile(filePath) {
            streamSlider.value = 0
            streamSlider.to = 0
            position.text = "00:00"
            duration.text = "00:00"
        }

        function onStreamPosChangeInFrames(streamPos) {
            streamSlider.value = streamPos
        }

        function onStreamPosChangeInSeconds(streamPos) {
            position.text = SAL.parseTime(streamPos)
        }
    }
}
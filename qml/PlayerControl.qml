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
        anchors.topMargin: 3
        anchors.bottomMargin: 3

        Button {
            id: previousBtn

            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight

            icon.source: "qrc:///images/previousSound.png"
            icon.width: Layout.preferredWidth - 4
            icon.color: enabled ? "#444" : "lightgray"
            enabled: false

            onClicked: PlaybackControlSystem.currentBackend === PlaybackControlSystem.SAL ? SAL.previous() : console.log("oups")
        }

        // Play pause button.
        PlayPauseButton {
            id: playBtn
            
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight

            // Play or pause when the button is clicked.
            onClicked: PlaybackControlSystem.currentBackend === PlaybackControlSystem.StreamBackend.SAL ? 
                        SAL.playPause() : 
                        PlaybackControlSystem.playPause()
        }

        // Stop button
        Button {
            id: stopBtn

            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight

            icon.source: "qrc:///images/stop.png"
            icon.width: Layout.preferredWidth - 4
            icon.color: enabled ? "#444" : "lightgray"
            enabled: false

            // Stop the stream.
            onClicked: PlaybackControlSystem.currentBackend === PlaybackControlSystem.StreamBackend.SAL ? SAL.stop() : console.log("oups3")
        }

        // Random button
        Button {
            id: randomBtn

            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight

            icon.source: "qrc:///images/rand.png"
            icon.width: Layout.preferredWidth - 4
            icon.color: "#444"
            checkable: true
            checked: PlaybackControlSystem.currentBackend === PlaybackControlSystem.StreamBackend.SAL ? SAL.isRandom : PlaybackControlSystem.isShuffled
            onClicked: PlaybackControlSystem.currentBackend === PlaybackControlSystem.StreamBackend.SAL ? SAL.setRandom(checked) : PlaybackControlSystem.isShuffled = this.checked
        }

        // Next button
        Button {
            id: nextBtn

            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: Layout.preferredHeight

            icon.source: "qrc:///images/nextSound.png"
            icon.width: Layout.preferredWidth - 4
            icon.color: enabled ? "#444" : "lightgray"
            enabled: false

            onClicked: PlaybackControlSystem.currentBackend == PlaybackControlSystem.StreamBackend.SAL ? 
                        SAL.next() : 
                        console.log("oups5")
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
                PlaybackControlSystem.seek(value)
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
        target: PlaybackControlSystem

        function onIsReadyChanged() {
            const isReady = PlaybackControlSystem.isReady;
            playBtn.enabled = isReady;
            streamSlider.enabled = isReady;
        }

        function onIsPlayingChanged() {
            const isPlaying = PlaybackControlSystem.isPlaying;
            if (isPlaying) {
                playBtn.playing();
            } else {
                playBtn.stopping();
            }
            previousBtn.enabled = isPlaying
            nextBtn.enabled = isPlaying
            stopBtn.enabled = isPlaying
        }

        function onIsPausedChanged() {
            if (PlaybackControlSystem.isPaused) {
                playBtn.stopping()
            } else {
                playBtn.playing();
            }
        }

        function onCurrentStreamChanged() {
            const filePath = PlaybackControlSystem.currentStream;
            const isValid = filePath.length > 0;

            streamSlider.value = 0;
            streamSlider.to = isValid ? PlaybackControlSystem.streamSize : 0;
            position.text = "00:00";
            duration.text = isValid ? SAL.parseTime(PlaybackControlSystem.streamSizeSeconds) : "00:00";

            if (PlaybackControlSystem.currentBackend === PlaybackControlSystem.StreamBackend.SAL) {
                TrackTag.filePath = filePath;
            }
        }

        function onStreamPosChanged() {
            streamSlider.value = PlaybackControlSystem.streamPos;
        }

        function onStreamPosSecondsChanged() {
            position.text = SAL.parseTime(PlaybackControlSystem.streamPosSeconds);
        }

        function onStreamSizeChanged() {
            streamSlider.to = PlaybackControlSystem.streamSize;
        }

        function onStreamSizeSecondsChanged() {
            duration.text = SAL.parseTime(PlaybackControlSystem.streamSizeSeconds);
        }
    }
}

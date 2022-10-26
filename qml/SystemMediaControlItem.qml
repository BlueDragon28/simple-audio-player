import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/simple-audio-library.js" as SAL

/*
A simple item to interface with the SystemMediaControl C++ interface.
*/
Item {
    id: root

    signal raise();

    // Connecting to the Player singleton object and listen to the signal emitted by the SAL library.
    Connections {
        target: Player

        // When the SAL is playing, notify the system.
        function onStreamPlaying() {
            SystemMediaControl.play();
        }

        // When the SAL is paused, notify the system.
        function onStreamPaused() {
            SystemMediaControl.pause();
        }

        // When the SAL is stopped, notify the system.
        function onStreamStopping() {
            SystemMediaControl.stop();
        }

        // When the current stream is changed, notify the system.
        function onCurrentStreamChanged() {
            SystemMediaControl.newTrack({
                index: 0,
                path: Player.currentStream
            });

            // If there is another song after this one, notify mpris.
            SystemMediaControl.canNext(
                PlayingList.hasNext());
        }
    }

    // Connecting to the TrackTack singleton object to listen to track change
    Connections {
        target: TrackTag

        // If the title change, it must mean that every thing else has changed.
        function onTitleChanged() {
            /*
            Retrieve information on the title.
            */
            SystemMediaControl.newTrack({
                index: 0, // No index yet.
                title: TrackTag.title,
                album: TrackTag.album,
                artists: TrackTag.artist
            });
        }
    }

    // Connecting to the SystemMediaControl C++ class.
    Connections {
        target: SystemMediaControl

        // When the System Media Control ask the window to be raised.
        function onRaise() {
            root.raise();
        }

        // When the System Media Control ask to PlayPause.
        function onPlayPause() {
            SAL.playPause();
        }

        // Restart or going to the previous track.
        function onPrevious() {
            SAL.previous();
        }

        // Going to the next track.
        function onNext() {
            SAL.next();
        }
    }

    Component.onCompleted: console.log("Completed");
}
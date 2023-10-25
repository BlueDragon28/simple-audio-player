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
        target: PlaybackControlSystem

        // When the stream is playing, notify the system.
        function onIsPlayingChanged() {
            if (PlaybackControlSystem.currentBackend === PlaybackControlSystem.SAL) {
                if (PlaybackControlSystem.isPlaying === true) {
                    SystemMediaControl.play();
                }
            } else {
                if (PlaybackControlSystem.isPlaying === true) {
                    SystemMediaControl.play();
                } else {
                    SystemMediaControl.pause();
                }
            }
        }

        // When the stream is pausing, notify the system.
        function onIsPausedChanged() {
            if (PlaybackControlSystem.isPaused === true) {
                SystemMediaControl.pause();
            } else if (
                PlaybackControlSystem.isPaused === false &&
                PlaybackControlSystem.isPlaying === true
            ) {
                SystemMediaControl.play();
            }
        }

        // When the stream is stopped, notify the system.
        function onIsRunningChanged() {
            if (PlaybackControlSystem.isRunning === false) {
                SystemMediaControl.stop();
            }
        }

        // When the current stream is changed, notify the system.
        function onCurrentStreamChanged() {
            if (PlaybackControlSystem.currentBackend !== PlaybackControlSystem.SAL) return;

            // If there is another song after this one, notify mpris.
            SystemMediaControl.canNext(
                PlayingList.hasNext());
        }

        function onTrackNameChanged() {
            if (PlaybackControlSystem.currentBackend !== PlaybackControlSystem.SPOTIFY) return;

            SystemMediaControl.newTrack({
                index: 0,
                title: PlaybackControlSystem.trackName,
                album: PlaybackControlSystem.trackAlbumName,
                artists: PlaybackControlSystem.trackArtists,
            });

            SystemMediaControl.canNext(true);
        }
    }

    // Connecting to the TrackTack singleton object to listen to track change
    Connections {
        target: TrackTag

        // If the title change, it must mean that every thing else has changed.
        function onTitleChanged() {
            if (PlaybackControlSystem.currentBackend === PlaybackControlSystem.SPOTIFY) return;

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
}

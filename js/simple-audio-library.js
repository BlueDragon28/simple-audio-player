/*
Wrapper to the SAL C++ module.
*/

.import SAL 1.0 as SAL

function open(filePath) {
    if (SAL.Player.isReadable(filePath)) {
        SAL.Player.open(filePath)
        SAL.Player.play()
    }
}

// Function to play or pause when the play/pause button is clicked.
function playPause() {
    if (SAL.Player.isPlaying()) {
        SAL.Player.pause()
    } else if (SAL.Player.isReady()) {
        SAL.Player.play()
    }
}
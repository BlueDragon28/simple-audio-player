/*
Wrapper to the SAL C++ module.
*/

.import SAL 1.0 as SAL

function open(filePath) {
    SAL.Player.open(filePath)
    if (SAL.Player.isReady()) {
        SAL.Player.play()
    }
}
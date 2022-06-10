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
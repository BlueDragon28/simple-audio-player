/*
Wrapper to the SAL C++ module.
*/

.import SimpleAudioPlayer 1.0 as SAP
.import SAL 1.0 as SAL

function open(filePath) {
    if (typeof filePath === "string") {
        if (SAL.Player.isReadable(filePath)) {
            SAL.Player.open(filePath)
            SAL.Player.play()
        }
    } else {
        let validFiles = new Array()
        for (let i = 0; i < filePath.length; i++) {
            if (SAL.Player.isReadable(filePath[i])) {
                validFiles.push(filePath[i])
            }
        }
        SAP.PlayingList.list = validFiles
        SAL.Player.open(validFiles)
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

/*
Function parsing seconds into a string of hours, minutes and seconds.
*/
function parseTime(time) {
    var strTime = "";

    // Parse hours.
    if (time >= 3600) {
        let t = String(Math.floor(time / 3600))
        time = Math.floor(time % 3600)
        if (t.length == 1) {
            t = "0" + t
        }
        strTime += t + ":"
    }

    // Parse minutes.
    if (time >= 60) {
        let t = String(Math.floor(time / 60))
        time = Math.floor(time % 60)
        if (t.length == 1) {
            t = "0" + t
        }
        strTime += t + ":"
    }
    
    // Parse seconds.
    let t = String(time)
    if (strTime.length == 0) {
        strTime += "00:"
    }
    if (t.length == 1) {
        t = "0" + t
    }
    strTime += t

    return strTime
}
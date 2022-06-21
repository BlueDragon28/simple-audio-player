/*
Wrapper to the SAL C++ module.
*/

.import SimpleAudioPlayer 1.0 as SAP
.import SAL 1.0 as SAL

// Open a new list.
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
        // Prevent the onEndFile signal to change the PlayingList index.
        // The property is set back to false by the onEndFile signal.
        SAP.PlayingList.doNotMove = true
        SAL.Player.open(validFiles)
        SAL.Player.play()
    }
}

// Time since the last time the previous function was called.
let lastTime = -1

// Retart the current track or if the user double click, go to the previous track.
function previous() {
    let currentTime = new Date().getTime()
    /*
    If the last time is equal to -1, this mean that the previous function is called for the first time.
    If more that 200 ms passed since the last time, seek to the beginning of the track,
    otherwise, go to the previous track.
    */
    if (lastTime == -1 || currentTime - lastTime > 1000) {
        SAL.Player.seek(0)
        lastTime = currentTime
    } else if (SAP.PlayingList.hasPrevious()) {
        SAP.PlayingList.previous()
        SAP.PlayingList.doNotMove = true
        SAL.Player.open(SAP.PlayingList.listFromIndex())
        SAL.Player.play()
        lastTime = currentTime
    }
}

// Move to the next track.
function next() {
    if (SAP.PlayingList.hasNext()) {
        SAP.PlayingList.next()
        SAP.PlayingList.doNotMove = true
        SAL.Player.open(SAP.PlayingList.listFromIndex())
        SAL.Player.play()
    }
}

// Play or pause when the play/pause button is clicked.
function playPause() {
    if (SAL.Player.isPlaying()) {
        SAL.Player.pause()
    } else if (SAL.Player.isReady()) {
        SAL.Player.play()
    }
}

// Parsing seconds into a string of hours, minutes and seconds.
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
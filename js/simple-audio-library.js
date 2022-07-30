/*
Wrapper to the SAL C++ module.
*/

.pragma library
.import SimpleAudioPlayer 1.0 as SAP

// Random the playing list.
let isRandom = false;
function setRandom(value) {
    if (typeof value === "boolean") {
        isRandom = value
    }
}

// Using the shuffle algorithm to randomize an array
function shuffle(array) {
    for (let i = array.length-1; i > 0; i--) {
        var randomIndex = Math.floor(Math.random() * (i + 1));
        [array[i], array[randomIndex]] = [array[randomIndex], array[i]];
    }
    return array
}

// Randomize the playing list.
function randomize(playingList, firstToPlay) {
    // If playingList if a string, return it.
    if (typeof playingList === "string") {
        return playingList
    } else {
        // Check if first to play is in the list.
        let isInTheList = false
        if (firstToPlay.length > 0) {
            for (let i = 0; i < playingList.length; i++) {
                if (playingList[i] === firstToPlay) {
                    playingList.splice(i, 1)
                    isInTheList = true
                    break
                }
            }
        }   

        // Randomize the playingList array
        let randomArray = shuffle(playingList)
        // Add the firstToPlay (if exist) to the beguinning of the list.
        if (isInTheList) {
            randomArray.splice(0, 0, firstToPlay)
        }

        return randomArray
    }
}

// Open a new list.
function open(filePath, firstElement = "") {
    if (typeof filePath === "string") {
        if (SAP.Player.isReadable(filePath)) {
            SAP.Player.open(filePath)
            SAP.Player.play()
        }
    } else if (filePath.length > 0) {
        let validFiles = new Array()
        for (let i = 0; i < filePath.length; i++) {
            if (SAP.Player.isReadable(filePath[i])) {
                validFiles.push(filePath[i])
            }
        }

        // Randomize the array if isRandom is true
        if (isRandom) {
            validFiles = randomize(validFiles, firstElement)
        }

        SAP.PlayingList.list = validFiles
        if (firstElement.length > 0) {
            SAP.PlayingList.next(firstElement) // Move the list to the selected item.
        }
        SAP.Player.open(SAP.PlayingList.listFromIndex())
        SAP.Player.play()
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
        SAP.Player.seek(0)
        lastTime = currentTime
    } else if (SAP.PlayingList.hasPrevious()) {
        SAP.PlayingList.previous()
        SAP.Player.open(SAP.PlayingList.listFromIndex())
        SAP.Player.play()
        lastTime = currentTime
    }
}

// Move to the next track.
function next() {
    if (SAP.PlayingList.hasNext()) {
        SAP.PlayingList.next()
        SAP.Player.next()
    }
}

// Play or pause when the play/pause button is clicked.
function playPause() {
    if (SAP.Player.isPlaying()) {
        SAP.Player.pause()
    } else if (SAP.Player.isReady()) {
        SAP.Player.play()
    }
}

// Stop the stream.
function stop() {
    SAP.Player.stop()
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

/*
Play selected music
*/
function playSelectedMusic(model) {
    // Retrive the list of selected items.
    let selectedItems = model.selectedFilesList()

    // If any items in the list, open and play the list.
    if (selectedItems.length > 0) {
        open(selectedItems)
    }
}

/*
Retrieving the filename from a path.
*/
function getFileName(path) {
    let index = path.lastIndexOf("/")

    if (index >= 0) {
        if (index+1 < path.length) {
            return path.slice(index+1)
        } else {
            return ""
        }
    } else {
        return path
    }
}

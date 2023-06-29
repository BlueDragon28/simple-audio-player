const minimumInterval = 1000; // milliseconds
let lastTime = -1;

function getCurrentTime() {
    return Date.now();
}

function canGoToPrevious() {
    const currentTime = getCurrentTime();
    const canGoToNext = lastTime !== -1 && currentTime - lastTime <= minimumInterval;
    lastTime = currentTime;
    return canGoToNext;
}

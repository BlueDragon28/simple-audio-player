/*
Helper method to handle config option.
*/

.pragma library
.import QtQuick 6.2 as QtQuick
.import QtQuick.Controls 6.2 as QtControl
.import SimpleAudioPlayer 1.0 as SAP

// Retrieve informations about the status of the application window.
function saveApplicationWindowStatus(window) {
    if (window && typeof window === "object") {
        // Store the status of the window in an object
        const status = {
            x: window.x,
            y: window.y,
            width: window.width,
            height: window.height,
            miximize: window.visibility === QtQuick.Window.Maximized
        };

        // and set it to the AppConfig C++ singleton class.
        SAP.AppConfig.setMainWindowSettings(status);
    }
}

// Getting informations about the application window status.
function loadApplicationWindowStatus() {
    return SAP.AppConfig.getMainWindowSettings();
}
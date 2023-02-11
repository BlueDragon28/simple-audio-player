import QtCore 6.2
import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

Item {
    id: root

    signal accepted(string paths)

    function open() {
        fileDialog.open()
    }

    SystemFileDialog {
        id: fileDialog
        title: "Save Playlist"
        fileMode: SystemFileDialog.FileMode.SAVE_FILE
        directory: StandardPaths.standardLocations(StandardPaths.MusicLocation)[0]
        defaultSuffix: "json"
        nameFilters: ["JSON (*.json)", "All Files (*)"]

        onAccepted: root.accepted(fileDialog.selectedFile)
    }
}

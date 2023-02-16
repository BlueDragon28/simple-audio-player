import QtCore 6.2
import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

Item {
    id: root

    property alias title: fileDialog.title
    property string mode: ""
    property alias directory: fileDialog.directory
    property alias defaultSuffix: fileDialog.defaultSuffix
    property alias nameFilters: fileDialog.nameFilters

    signal accepted(string paths)

    function open() {
        fileDialog.open()
    }

    SystemFileDialog {
        id: fileDialog
        fileMode: mode === "SAVE" ? SystemFileDialog.SAVE_FILE : mode === "OPEN" ? SystemFileDialog.OPEN_FILE : SystemFileDialog.OPEN_FILE

        onAccepted: root.accepted(fileDialog.selectedFile)
    }
}

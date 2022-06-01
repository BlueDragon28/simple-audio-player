import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

/*
This line edit is used to display and edit the file path of the filesystem
navigation display.
*/
FocusScope {
    id: root

    property alias text: pathEdit.text
    signal accepted(string path)

    // The text field displaying the folder path.
    TextField {
        id: pathEdit
        anchors.fill: parent
        verticalAlignment: Qt.AlignVCenter
        selectByMouse: true
        focus: true

        // When accepted (when the user press enter key),
        // the accepted signal of root item is called.
        onAccepted: root.accepted(text)
    }
}
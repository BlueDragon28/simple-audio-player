import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
This is the content of the MainWindow.
*/
Item {
    id: root

    RowLayout {
        anchors.fill: parent
        spacing: 5

        SidePanel {
            id: sidePanel
            width: 200
            Layout.fillHeight: true
        }

        AudioFileContent {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
This is the content of the MainWindow.
This is the item displayed inside the main window.
Its displaying a side panel and an audio content view.
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

            /*
            Bind the sectionType of sidePanel to this one.
            Every time the sectionType of sidePanel will change,
            the content of AudioFile will also change.
            */
            sectionType: sidePanel.sectionType
        }
   }
}
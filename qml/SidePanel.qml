import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/simple-audio-library.js" as SAL

/*
This is the side panel of the audio player.
Ths user can select from a list of content (filesystem, music...).
*/
Item {
    id: root

    // Store the type of the selected item.
    property int sectionType: sectionList.model.get(sectionList.currentIndex).type

    Rectangle {
        anchors.fill: parent
        color: "white"

        ColumnLayout {
            anchors.fill: parent

            // Section list.
            ListView {
                id: sectionList
                /*anchors.fill: parent
                anchors.margins: 8*/
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 8

                // Model with the section list.
                model: SectionListModel {}

                // Delegate displaying the section list name.
                delegate: Label {
                    text: name
                    width: sectionList.width

                    MouseArea {
                        anchors.fill: parent
                        onClicked: sectionList.currentIndex = index
                    }
                }

                // Rectangle displaying the current selection.
                highlight: Rectangle {
                    color: "lightsteelblue"
                }
            }

            /*
            Track information
            */
            TrackInformation {
                id: trackInformation

                Layout.fillWidth: parent
            }
        }
    }

    // Connection to the Player C++ class to receive signals calls.
    Connections {
        target: Player

        // When a new stream start, update trackInformation with the name of the file.
        function onStartNewFile(filePath) {
            trackInformation.name = SAL.getFileName(filePath)
        }

        // When a stream stop, reset trackInformation.
        function onEndFile(filePath) {
            trackInformation.name = ""
        }
    }
}
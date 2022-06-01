import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

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

        // Section list.
        ListView {
            id: sectionList
            anchors.fill: parent
            anchors.margins: 8

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
    }
}
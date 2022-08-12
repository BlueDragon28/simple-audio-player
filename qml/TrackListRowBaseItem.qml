import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
A single row of a tracks list view. It is displaying the track name
*/
Rectangle {
    id: root
    height: contentHeight+9

    // When the row is selected, fill the rectangle with lightsteelblue color.
    color: isItemSelected ? "lightsteelblue" : "transparent"

    // Does the row is selected.
    property bool isItemSelected: false

    // Does the track is playing.
    property bool isPlaying: false

    // Redirect the children to the rowLine item.
    default property alias children: rowLine.children

    // Height of the content inside the rowLine item.
    property real contentHeight: 0

    // Signal triggered when the MouseArea is cliked.
    signal clicked(QtObject mouse)

    // Signal triggered when the MouseArea is double clicked.
    signal doubleClicked(QtObject mouse)

    /*
    This rectangle is displayed if the file is playing.
    */
    Rectangle {
        anchors.fill: parent
        color: isPlaying ? Qt.color("#A00040FF") : "transparent"
        radius: 8

        /*
        Line row. In it, the children of the root item will be send here.
        */
        Row {
            id: rowLine
            width: parent.width-8
            height: parent.height-8
            spacing: 2
            x: 4
            y: 4

        }

        // Line at the bottom of the row.
        Rectangle {
            anchors.top: parent.bottom
            anchors.topMargin: -1
            anchors.left: parent.left

            width: parent.width
            height: 1
            color: "lightgray"
        }

        // Mouse area to change the selection in the list.
        MouseArea {
            anchors.fill: parent

            // Accept left, middle and right button events.
            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

            // When clicked, redirect the call to the cliked signal of the root item.
            onClicked: function(mouseEvent) {
                root.clicked(mouseEvent)
            }

            // When double clicked, redirect the call to the doubleClicked of the root item.
            onDoubleClicked: function(mouseEvent) {
                root.doubleClicked(mouseEvent)
            }
        }
    }
}
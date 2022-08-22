import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
This element display the informations about an album.
*/
Item {
    id: root
    height: 200 // Default height of the album header element.

    signal back() // Go back to the albums list.

    Column {
        anchors.fill: parent
        anchors.margins: 8

        // Button to allow to go back to the albums list.
        Button {
            id: goBack
            text: "<"
            width: height

            onClicked: root.back()
        }
    }
}

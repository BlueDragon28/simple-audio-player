import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
Header element of the album content list view header.
*/
Item {
    id: root
    height: 25

    property alias nameWidth: name.width
    property alias artistWidth: name.width

    ToolBar {
        anchors.fill: parent
        horizontalPadding: 0

        SplitView {
            anchors.fill: parent
            orientation: Qt.Horizontal

            // Track name column.
            Item {
                id: name

                SplitView.minimumWidth: 120
                SplitView.fillWidth: true

                Label {
                    anchors.fill: parent
                    text: "name"
                    clip: true
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }

            // Artist name
            Item {
                id: artist

                SplitView.minimumWidth: 120

                Label {
                    anchors.fill: parent
                    text: "artist"
                    clip: true
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }
        }
    }
}
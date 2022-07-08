import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
This item is the header of the fileSystemRow ListView element.
It display the name of the column and it allow the user to resize them.
*/
Item {
    id: root
    width: 500
    height: 25

    property int nameSize: name.width
    property int lastModifiedSize: lastModified.width
    property int sizeSize: size.width

    ToolBar {
        anchors.fill: parent
        horizontalPadding: 0

        SplitView {
            anchors.fill: parent
            orientation: Qt.Horizontal

            // The file name column.
            Item {
                id: name

                SplitView.minimumWidth: 120
                SplitView.fillWidth: true

                Label {
                    anchors.fill: parent
                    text: "Name"
                    clip: true

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            // The last time the file was modified.
            Item {
                id: lastModified

                SplitView.minimumWidth: 200
                SplitView.maximumWidth: 500

                Label {
                    anchors.fill: parent
                    text: "Last modified"
                    clip: true

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            // The file size column.
            Item {
                id: size

                SplitView.minimumWidth: 50
                SplitView.preferredWidth: 80
                SplitView.maximumWidth: 200

                Label {
                    anchors.fill: parent
                    text: "Size"
                    clip: true

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }
}
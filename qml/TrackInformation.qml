import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0
import "qrc:///js/simple-audio-library.js" as SAL

/*
Displaying informations about the current track.
*/
Item {
    id: root
    implicitHeight: rect.height

    Rectangle {
        id: rect
        width: parent.width
        height: horizontalLayout.height
        color: "lightgray"

        ColumnLayout {
            id: horizontalLayout
            width: parent.width

            // Displaying track informations in labels that slide when the text go beyond the boundaries.

            // Track title
            TrackLabel {
                id: trackTitle
                Layout.fillWidth: true
                Layout.margins: 8
            }
        }
    }

    // Connect to the TrackTag singleton to receive notification when tags are changed.
    Connections {
        target: TrackTag

        function onTitleChanged() {
            /*
                Display the title of the track.
                If the track doesn't have a title, use the file name.
            */
            let title = TrackTag.title
            let filePath = TrackTag.filePath

            if (title.length > 0) {
                trackTitle.text = title
            } else if (filePath.length == 0) {
                trackTitle.text = ""
            } else {
                trackTitle.text = SAL.getFileName(filePath)
            }
        }
    }
}
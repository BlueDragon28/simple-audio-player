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

            // This is a clipping item, it's used to hide every text that go beyond the boundaries of the available space.
            Item {
                clip: true
                Layout.fillWidth: true
                Layout.preferredHeight: trackName.height
                Layout.margins: 8

                Label {
                    id: trackName

                    // Every time the text of the label change, if the text go beyond the border, start the animation.
                    onTextChanged: {
                        moveAnimation.stop()
                        x = 0

                        if (width > parent.width) {
                            displacementAnim.to = -width // Update the displacementAnim.to property with the negative width of the label.
                            displacementAnim.duration = (width / 50) * 1000
                            moveAnimation.start()
                        }
                    }

                    // Moving the text to allow the user to see it.
                    SequentialAnimation on x {
                        id: moveAnimation
                        loops: Animation.Infinite
                        PauseAnimation { duration: 1000 }
                        PropertyAnimation { id: displacementAnim; from: 0 }
                        PauseAnimation { duration: 1000 }
                        PropertyAnimation { to: 0; duration: 0 }
                    }
                }
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
            if (title.length > 0) {
                trackName.text = title
            } else {
                trackName.text = getFileName(TrackTag.filePath)
            }
        }
    }
}
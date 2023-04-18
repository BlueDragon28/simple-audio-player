import QtQuick 6.2
import QtQuick.Controls 6.2

// Clipping item hiding the text that go beyond the boundaries.
Item {
    id: root
    clip: true
    implicitHeight: trackLabel.height
    property bool secondaryInfo: false

    // Get access to the label properties.
    property alias text: trackLabel.text
    property alias fontSize: trackLabel.font.pointSize
    property alias color: trackLabel.color

    Label {
        id: trackLabel
        color: secondaryInfo ? palette.dark : palette.text

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
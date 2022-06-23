import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2

/*
This slider is displaying the playing position of the audio file.
It allow the user to change the position in the stream.
*/
Item {
    id: root

    property alias to: slider.to
    property alias enabled: slider.enabled

    /*
    This property is updating the value properties of the slider.
    However, this property is doing nothing when the property pressed is true.
    */
    property real value: 0
    onValueChanged: {
        if (!slider.pressed) {
            slider.value = root.value
        }
    }

    signal moved()

    Slider {
        anchors.fill: parent
        id: slider
        enabled: false
        from: 0
        stepSize: 1

        property real newValue: 0
        property bool isNewValue: false

        onMoved: {
            isNewValue = true
            newValue = value
        }

        // Workaround to prevent the value to change all along when the user move the slider.
        onPressedChanged: {
            if (!pressed) {
                if (isNewValue) {
                    isNewValue = false
                    root.value = value
                    root.moved()
                }
            }
        }
    }
}
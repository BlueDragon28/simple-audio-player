import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

ToolBar {
    id: root
    implicitHeight: toolBarLayout.height + 8

    signal back()

    RowLayout {
        id: toolBarLayout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 4
        anchors.bottomMargin: 4
        width: parent.width
        height: 26

        Button {
            id: goBack
            icon.source: "qrc:///images/back.png"
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: height

            onClicked: root.back()
        }

        Label {
            Layout.preferredWidth: parent.width - goBack.implicitWidth
            Layout.preferredHeight: parent.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: "Playlist"
        }
    }
}
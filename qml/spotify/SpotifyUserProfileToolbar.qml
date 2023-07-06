import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

ToolBar {
    id: root
    implicitHeight: 34

    RowLayout {
        id: toolBarLayout
        anchors.fill: parent
        anchors.topMargin: 4
        anchors.bottomMargin: 4

        Button {
            id: logout
            text: "logout"
            Layout.fillHeight: true

            onClicked: SpotifyAPI.logout()
        }

        Label {
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: "User Profile"
        }
    }
}

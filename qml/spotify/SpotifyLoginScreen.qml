import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Item {
    id: root

    function clearField() {
        clientID.text = "";
    }

    Rectangle {
        anchors.centerIn: parent
        width: 300
        height: 200
        color: palette.alternateBase
        radius: 10

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15

            Label {
                Layout.fillWidth: true

                text: "Login to Spotify"
                font.pixelSize: 35
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                text: "enter your developer id"
            }

            TextField {
                Layout.fillWidth: true

                id: clientID
                verticalAlignment: Qt.AlignVCenter
                selectByMouse: true
            }

            CheckBox {
                id: saveCredentials
                text: "Save login details (may not be safe)"
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 120

                text: "Login"

                onClicked: function() {
                    SpotifyAPI.setClientID(clientID.text);
                    SpotifyAPI.authenticate(saveCredentials.checked);
                }
            }
        }
    }

    MessageDialog {
        id: errorDialog
        property string errorMessage: ""
        text: "Something Went Wrong!"
        informativeText: errorMessage
    }

    Connections {
        target: SpotifyAPI

        function onError(errorMessage) {
            errorDialog.errorMessage = errorMessage;
            errorDialog.open()
        }
    }
}

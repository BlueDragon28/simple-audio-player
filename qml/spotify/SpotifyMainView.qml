import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

Item {
    id: root

    Item {
        anchors.centerIn: parent

        Column {
            Label {
                text: "Login to spotify"
            }

            TextField {
                id: clientID
                verticalAlignment: Qt.AlignVCenter
                selectByMouse: true
            }

            Button {
                text: "Login"

                onClicked: function() {
                    SpotifyAPI.setClientID(clientID.text);
                    SpotifyAPI.authenticate();
                }
            }
        }
    }
}
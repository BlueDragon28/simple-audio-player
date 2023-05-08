import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import QtQuick.Dialogs 6.3
import SimpleAudioPlayer 1.0

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 158

            color: palette.alternateBase

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Image {
                    id: userProfileIcon
                    Layout.preferredWidth: 128
                    Layout.preferredHeight: 128
                    sourceSize.width: Layout.preferredWidth
                    sourceSize.height: Layout.preferredHeight
                    source: "qrc:///images/userProfile.svg"
                    smooth: true
                    mipmap: true
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    spacing: 0

                    Item {
                        Layout.fillHeight: true
                    }

                    Label {
                        text: SpotifyAPI.userInfo.username
                        font.pointSize: 30
                    }

                    Row {
                        Label {
                            text: "Country: " + SpotifyAPI.userInfo.country + ", Followers: " + SpotifyAPI.userInfo.followers
                            font.pointSize: 8
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
            }

            PreviousNextPlaylist {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            color: palette.base

            SpotifyUserPlaylist {
                anchors.fill: parent
            }
        }
    }
}
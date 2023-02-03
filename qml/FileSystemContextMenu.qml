import QtQuick 6.2
import QtQuick.Controls 6.2

/*
This is the context menu called when the user right click on the file system.
*/
Menu {
    id: root

    signal play()
    signal addToPlaylist()

    MenuItem {
        text: "Play"
        onTriggered: root.play()
    }

    MenuItem {
        text: "Add To Playlist"

        onTriggered: function() {
            root.addToPlaylist();
        }
    }
}

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import SimpleAudioPlayer 1.0

// This is where the audio file, album, etc... is available.
Item {
    id: root

    /*
    Property holding the section type. When it is changing,
    the signal handler change the state to update the loader.
    */
    property int sectionType: 0
    onSectionTypeChanged: {
        if (sectionType == SectionListModel.FILE_SYSTEM) {
            state = "File System"
        } else if (sectionType == SectionListModel.ALBUMS) {
            state = "Albums"
        } else {
            state = ""
        }
    }

    /*
    The states hold the sections of displayable.
    When the state change, the stack layout show the proper item.
    */
    states: [
        State {
            name: "File System"
            PropertyChanges { target: stackLayout; currentIndex: 0; }
        },
        State {
            name: "Albums"
            PropertyChanges { target: stackLayout; currentIndex: 1; }
        }
    ]

    /*
    The stack layout hold the section content.
    When the state change, the layout is changed to.
    */
    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: 0

        FileSystemView {}
        AlbumsSection {}
    }
}
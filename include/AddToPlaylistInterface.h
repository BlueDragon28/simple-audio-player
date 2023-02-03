#ifndef SIMPLEAUDIOPLAYER_ADDTOPLAYLISTINTERFACE_H_
#define SIMPLEAUDIOPLAYER_ADDTOPLAYLISTINTERFACE_H_

#include <QObject>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qurl.h>

/*
A simple interface to easily add track to the playlist.
*/
class AddToPlaylistInterface : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    AddToPlaylistInterface();
    ~AddToPlaylistInterface();

signals:
    void tracksAdded(const QStringList& tracksPath);

public slots:
    void addTracks(const QStringList& tracksPath);
};

#endif //SIMPLEAUDIOPLAYER_ADDTOPLAYLISTINTERFACE_H_

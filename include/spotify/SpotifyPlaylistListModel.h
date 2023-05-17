#ifndef SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTLISTMODEL_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTLISTMODEL_H_

#include "SelectionModel.h"
#include "spotify/SpotifyReceivedPlaylistElement.h"
#include <qnamespace.h>
#include <qtmetamacros.h>
#include <qqmlintegration.h>

class SpotifyPlaylistListModel : public SelectionModel 
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QUrl imageHref READ imageHref NOTIFY imageHrefChanged)
    Q_PROPERTY(QString id READ id NOTIFY id)

public:
    enum TrackRole 
    {
        NAME = MAX_ROLE,
        ARTISTS,
        ALBUM
    };
    Q_ENUM(TrackRole)

    SpotifyPlaylistListModel(QObject* parent = nullptr);
    virtual ~SpotifyPlaylistListModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    QString name() const;
    QUrl imageHref() const;
    QString id() const;

public slots:
    void setPlaylist(SpotifyReceivedPlaylistElement* playlist);

signals:
    void nameChanged();
    void imageHrefChanged();
    void idChanged();

signals:
    void playlistLoaded();
    
private:
    SpotifyReceivedPlaylistElement* m_playlist;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTLISTMODEL_H_
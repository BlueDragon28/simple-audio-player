#ifndef SIMPLEAUDIOPLAYER_SPOTIFYUSERPLAYLISTMODEL_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYUSERPLAYLISTMODEL_H_

#include "SpotifyPlaylist.h"
#include <qabstractitemmodel.h>
#include <qnamespace.h>
#include <qqmlintegration.h>
#include <qstring.h>
#include <qurl.h>

class SpotifyUserPlaylistsListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(SpotifyPlaylist* playlistsAPI READ playlistsAPI WRITE setPlaylistsAPI NOTIFY playlistsAPIChanged)

public:
    enum PlaylistRole 
    {
        TITLE = Qt::UserRole+1,
        IMAGE_URL,
        HREF,
        ID,
    };

    SpotifyUserPlaylistsListModel(QObject* parent = nullptr);
    virtual ~SpotifyUserPlaylistsListModel();

    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    SpotifyPlaylist* playlistsAPI() const;

public slots:
    QVariantMap get(int i) const;
    void setPlaylistsAPI(SpotifyPlaylist* playlistsAPI);

signals:
    void playlistsAPIChanged();

protected:
    struct PlaylistItem
    {
        QString title;
        QUrl imageUrl;
        QUrl href;
        QString id;
    };

private:
    void retrievePlaylists();
    void coverImageReceived(const QString& id, const QUrl& imagePath);

    SpotifyPlaylist* m_playlistApiInfo;
    QList<QVariant> m_items;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYUSERPLAYLISTMODEL_H_

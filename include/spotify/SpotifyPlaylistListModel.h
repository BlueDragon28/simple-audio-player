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
    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    Q_PROPERTY(QString uri READ uri NOTIFY uriChanged)
    Q_PROPERTY(QString authors READ authors NOTIFY authorsChanged)
    Q_PROPERTY(QString duration READ duration NOTIFY durationChanged)

public:
    enum TrackRole 
    {
        TRACK_NUMBER = MAX_ROLE,
        NAME,
        ARTISTS,
        ALBUM,
        URI
    };
    Q_ENUM(TrackRole)

    SpotifyPlaylistListModel(QObject* parent = nullptr);
    virtual ~SpotifyPlaylistListModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    QString name() const;
    QUrl imageHref() const;
    QString id() const;
    QString uri() const;
    QString authors() const;
    QString duration() const;

public slots:
    void setPlaylist(SpotifyReceivedPlaylistElement* playlist);

signals:
    void nameChanged();
    void imageHrefChanged();
    void idChanged();
    void uriChanged();
    void authorsChanged();
    void durationChanged();
    void playlistLoaded();
    
private:
    void setAuthors(const QString& authors);
    void setDuration(const QString& duration);
    void getAuthorsAndDuration();
    void parsePlaylistData();
    QString parseDuration(uint64_t duration) const;
    void coverImageReceived(const QString& id, const QUrl& imagePath);

    SpotifyReceivedPlaylistElement* m_playlist;
    QString m_authors;
    QString m_duration;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYLISTLISTMODEL_H_

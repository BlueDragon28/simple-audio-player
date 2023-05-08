#ifndef SIMPLEAUDIOPLAYER_SPOTIFYPLAYLIST_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYPLAYLIST_H_

#include "SpotifyPlaylistItem.h"
#include <qobject.h>
#include <qurl.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qnetworkreply.h>

class SpotifyPlaylist : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int total READ total NOTIFY totalChanged)
    Q_PROPERTY(int pageNumber READ pageNumber NOTIFY pageNumberChanged)
    Q_PROPERTY(int totalPages READ totalPages NOTIFY totalPagesChanged)
    Q_PROPERTY(int numberOnPage READ numberOnPage NOTIFY numberOnPageChanged)
    Q_PROPERTY(QList<QObject*>* playlists READ playlists NOTIFY playlistsChanged)
    Q_PROPERTY(bool hasNext READ hasNext NOTIFY hasNextChanged)
    Q_PROPERTY(bool hasPrevious READ hasPrevious NOTIFY hasPreviousChanged)

public:
    static const int MAX_PER_PAGE;
    static const QString SPOTIFY_USER_PLAYLIST_ENDPOINT;
    static const int LIMIT_IMAGE_HEIGHT;

    SpotifyPlaylist(QObject* parent = nullptr);
    virtual ~SpotifyPlaylist();

    int total() const;
    int pageNumber() const;
    int totalPages() const;
    int numberOnPage() const;
    QList<QObject*>* playlists();

    QUrl next() const;
    bool hasNext() const;
    QUrl previous() const;
    bool hasPrevious() const;

public slots:
    void handleFetchResponse(QNetworkReply* reply);
    void fetchNext();
    void fetchPrevious();

signals:
    void error();
    void totalChanged();
    void pageNumberChanged();
    void totalPagesChanged();
    void numberOnPageChanged();
    void playlistsChanged();
    void playlistLoaded();
    void hasNextChanged();
    void hasPreviousChanged();
    void fetchNextPage(const QUrl& url);
    void fetchPreviousPage(const QUrl& previous);

private:
    void setTotal(int total);
    void setPageNumber(int pageNumber);
    void setTotalPages(int totalPages);
    void setNumberOnPage(int numberOnPage);
    void setPlaylists(const QList<QObject*>& playlists);
    void destroyPlaylistsObjects();

    void setNext(const QString& nextUrl);
    void setPrevious(const QString& previousUrl);
    void setHasNext(const QUrl& url);
    void setHasPrevious(const QUrl& url);

    SpotifyPlaylistItem* parsePlaylistJSONItem(const QJsonValue& jItem);
    QUrl parseImagesUrls(const QJsonValue& imagesArrayValue);

    int m_total;
    int m_pageNumber;
    int m_totalPages;
    int m_numberOnPage;
    QList<QObject*> m_playlists;

    QUrl m_next;
    QUrl m_previous;
    bool m_hasNext;
    bool m_hasPrevious;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYLIST_H_

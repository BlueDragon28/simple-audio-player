#ifndef SIMPLEAUDIOPLAYER_SPOTIFYRECEIVEDPLAYLISTELEMENT_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYRECEIVEDPLAYLISTELEMENT_H_

#include <qjsonarray.h>
#include <qobject.h>
#include <qurl.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class SpotifyReceivedPlaylistElement : public QObject
{
    Q_OBJECT

public:
    struct Track
    {
        QString album;
        QString artists;
        QString id;
        QString name;
        QString href;
        int64_t durationMS;
    };

    SpotifyReceivedPlaylistElement(QObject* parent = nullptr);
    SpotifyReceivedPlaylistElement(const QByteArray& response, QObject* parent = nullptr);
    virtual ~SpotifyReceivedPlaylistElement();

    void parseResponse(const QByteArray& response);

    bool failed() const;
    QString name() const;
    QUrl imageHref() const;
    QString id() const;
    int tracksCount() const;
    Track track(int index) const;

private:
    void parsePlaylist(const QJsonObject& rootPlaylist);
    QUrl getImageHRef(const QJsonValue& jsonImagesArray);
    QList<Track> parseTracks(const QJsonObject& tracksJSonArray);
    Track parseTrack(const QJsonObject& trackObject, bool* error = nullptr);
    QString retrieveArtistsName(const QJsonArray& artistsArray, bool* error = nullptr);
    QString retrieveAlbumName(const QJsonObject& albumObject, bool* error = nullptr);

    bool m_failed;
    QString m_name;
    QUrl m_imageHref;
    QString m_id;
    QList<Track> m_tracks;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYRECEIVEDPLAYLISTELEMENT_H_
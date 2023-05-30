#include "spotify/SpotifyReceivedPlaylistElement.h"
#include "spotify/SpotifyPlaylist.h"
#include <cstdint>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>

SpotifyReceivedPlaylistElement::SpotifyReceivedPlaylistElement(QObject* parent) :
    QObject(parent),
    m_failed(false)
{}

SpotifyReceivedPlaylistElement::SpotifyReceivedPlaylistElement(const QByteArray& response, QObject* parent) :
    QObject(parent),
    m_failed(false)
{
    parseResponse(response);
}

SpotifyReceivedPlaylistElement::~SpotifyReceivedPlaylistElement()
{}

bool SpotifyReceivedPlaylistElement::failed() const
{
    return m_failed;
}

QString SpotifyReceivedPlaylistElement::name() const
{
    return m_name;
}

QUrl SpotifyReceivedPlaylistElement::imageHref() const
{
    return m_imageHref;
}

QString SpotifyReceivedPlaylistElement::id() const
{
    return m_id;
}

QString SpotifyReceivedPlaylistElement::uri() const
{
    return m_uri;
}

int SpotifyReceivedPlaylistElement::tracksCount() const
{
    return m_tracks.size();
}

SpotifyReceivedPlaylistElement::Track SpotifyReceivedPlaylistElement::track(int index) const
{
    return m_tracks.at(index);
}

void SpotifyReceivedPlaylistElement::parseResponse(const QByteArray& response)
{
    const QJsonDocument jsonDocument = QJsonDocument::fromJson(response);

    if (!jsonDocument.isObject()) 
    {
        m_failed = true;
        qDebug() << "playlist json data is not an object";
        return;
    }

    parsePlaylist(jsonDocument.object());
}

void SpotifyReceivedPlaylistElement::parsePlaylist(const QJsonObject& rootPlaylist)
{
    if (rootPlaylist.isEmpty() ||
        rootPlaylist.contains("error") ||
        !rootPlaylist.contains("name"))
    {
        m_failed = true;
        qDebug() << "root object empty or name key does not exists!";
        return;
    }

    const QString name = rootPlaylist.value("name").toString();

    if (name.isEmpty() || !rootPlaylist.contains("id"))
    {
        m_failed = true;
        qDebug() << "name is invalid or id does not exists!";
        return;
    }

    const QString id = rootPlaylist.value("id").toString();

    if (id.isEmpty() || !rootPlaylist.contains("uri"))
    {
        m_failed = true;
        qDebug() << "id is invalid or uri does not exists!";
        return;
    }

    const QString uri = rootPlaylist.value("uri").toString();

    if (uri.isEmpty() || !rootPlaylist.contains("images"))
    {
        m_failed = true;
        qDebug() << "uri is invalid or images does not exists!";
        return;
    }

    const QUrl imageHRef = getImageHRef(rootPlaylist.value("images").toArray());

    if (!imageHRef.isValid() || !rootPlaylist.contains("tracks"))
    {
        m_failed = true;
        qDebug() << "image is invalid or tracks does not exists!";
        return;
    }

    const QList<Track> tracksArray = parseTracks(rootPlaylist.value("tracks").toObject());

    if (tracksArray.isEmpty())
    {
        m_failed = true;
        qDebug() << "tracks is empty";
        return;
    }

    m_name = name;
    m_imageHref = imageHRef;
    m_id = id;
    m_uri = uri;
    m_tracks = tracksArray;
    m_failed = false;
}

QUrl SpotifyReceivedPlaylistElement::getImageHRef(const QJsonValue& jsonImagesArray)
{
    return SpotifyPlaylist::parseImagesUrls(jsonImagesArray);
}

QList<SpotifyReceivedPlaylistElement::Track> SpotifyReceivedPlaylistElement::parseTracks(const QJsonObject& tracksJsonObject)
{
    if (tracksJsonObject.isEmpty() || !tracksJsonObject.contains("items"))
    {
        qDebug() << "tracks json object is empty or items does not exists";
        return {};
    }

    const QJsonArray itemsJsonArray = tracksJsonObject.value("items").toArray();

    if (itemsJsonArray.isEmpty())
    {
        qDebug() << "tracks items json array is empty";
        return {};
    }

    QList<Track> playlistTracks;

    for (const QJsonValue& trackValue : itemsJsonArray)
    {
        if (!trackValue.isObject())
        {
            continue;
        }

        bool result = false;
        Track track = parseTrack(trackValue.toObject(), &result);

        if (result) continue;

        playlistTracks.append(track);
    }

    return playlistTracks;
}

SpotifyReceivedPlaylistElement::Track SpotifyReceivedPlaylistElement::parseTrack(const QJsonObject& trackObject, bool* error)
{
    const auto callError = [error] () -> Track {
        if (error) *error = true;
        return {};
    };

    if (trackObject.isEmpty() || !trackObject.contains("is_local"))
    {
        qDebug() << "isLocal not valid";
        return callError();
    }

    const bool isLocalFile = trackObject.value("is_local").toBool(true);

    if (isLocalFile || !trackObject.contains("track"))
    {
        qDebug() << "trackIsInvalid";
        return callError();
    }

    const QJsonValue trackJSonValue = trackObject.value("track");

    if (trackJSonValue.isNull() || !trackJSonValue.isObject())
    {
        qDebug() << "invalid track object";
        return callError();
    }

    const QJsonObject trackInfoObject = trackJSonValue.toObject();

    if (!trackInfoObject.contains("id"))
    {
        qDebug() << "no id";
        return callError();
    }

    const QString id = trackInfoObject.value("id").toString();

    if (id.isEmpty() || !trackInfoObject.contains("uri"))
    {
        qDebug() << "no uri";
        return callError();
    }

    const QString uri = trackInfoObject.value("uri").toString();

    if (uri.isEmpty() || !trackInfoObject.contains("name"))
    {
        qDebug() << "no name";
        return callError();
    }

    const QString name = trackInfoObject.value("name").toString();

    if (name.isEmpty() || !trackInfoObject.contains("href"))
    {
        qDebug() << "no href";
        return callError();
    }

    const QString href = trackInfoObject.value("href").toString();

    if (href.isEmpty() || !trackInfoObject.contains("duration_ms"))
    {
        qDebug() << "no duration";
        return callError();
    }

    const int64_t durationMS = trackInfoObject.value("duration_ms").toInteger(-1);

    if (durationMS < 0 || 
        !trackInfoObject.contains("artists") ||
        !trackInfoObject.value("artists").isArray())
    {
        qDebug() << "no artists";
        return callError();
    }

    bool getArtistsFailed;
    const QString artistsName = retrieveArtistsName(trackInfoObject.value("artists").toArray(), &getArtistsFailed);

    if (getArtistsFailed || 
        !trackInfoObject.contains("album") ||
        !trackInfoObject.value("album").isObject())
    {
        qDebug() << "no album";
        return callError();
    }

    bool getAlbumFailed;
    const QString albumName = retrieveAlbumName(trackInfoObject.value("album").toObject(), &getArtistsFailed);

    if (getAlbumFailed)
    {
        qDebug() << "failed to get album";
        return callError();
    }

    Track track;
    track.album = albumName;
    track.artists = artistsName;
    track.id = id;
    track.name = name;
    track.href = href;
    track.durationMS = durationMS;

    if (error) *error = false;
    return track;
}

QString SpotifyReceivedPlaylistElement::retrieveArtistsName(const QJsonArray& artistsArray, bool* error)
{
    const auto callError = [error]() -> QString {
        if (error) *error = true;
        return QString();
    };

    if (artistsArray.isEmpty()) 
    {
        return callError();
    }

    QString artistsName;

    for (const QJsonValue& artistValue : artistsArray)
    {
        if (!artistValue.isObject())
        {
            return callError();
        }

        const QJsonObject artistObject = artistValue.toObject();

        if (!artistObject.contains("name"))
        {
            return callError();
        }

        const QString artistName = artistObject.value("name").toString();

        if (artistName.isEmpty())
        {
            return callError();
        }

        artistsName += (artistsName.size() > 0 ? ", " : "") + artistName;
    }

    if (artistsName.isEmpty())
    {
        return callError();
    }

    if (error) *error = false;

    return artistsName;
}

QString SpotifyReceivedPlaylistElement::retrieveAlbumName(const QJsonObject& albumObject, bool* error)
{
    const auto callError = [error]() -> QString {
        if (error) *error = true;
        return QString();
    };

    if (albumObject.isEmpty() || !albumObject.contains("name"))
    {
        return callError();
    }

    const QString albumName = albumObject.value("name").toString();

    if (albumName.isEmpty())
    {
        return callError();
    }

    if (error) *error = false;
    return albumName;
}

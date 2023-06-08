#include "spotify/SpotifyPlaybackStatus.h"
#include "SpotifyAuthorizationPKCE.h"
#include "SpotifyPlayer.h"
#include "SpotifyReceivedPlaylistElement.h"
#include <cstdint>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkreply.h>
#include <qurlquery.h>

int SpotifyPlaybackStatus::_timerInterval = 5000;
const QString SpotifyPlaybackStatus::_packbackStateEndpointUrl =
    "https://api.spotify.com/v1/me/player";

SpotifyPlaybackStatus::SpotifyPlaybackStatus(
        SpotifyAuthorizationPKCE* spotifyAuth, 
        SpotifyPlayer* player, QObject* parent) :

    QObject(parent),
    m_fetchStatusTimer(new QTimer()),
    m_spotifyAuth(spotifyAuth),
    m_spotifyPlayer(player),
    m_isPlaying(false),
    m_progressMS(0),
    m_trackDurationMS(0)
{
    connect(m_fetchStatusTimer, &QTimer::timeout, this, &SpotifyPlaybackStatus::fetchStatus);
    connect(m_spotifyPlayer, &SpotifyPlayer::isPlaying, this, &SpotifyPlaybackStatus::playbackStartPlaying);
}

SpotifyPlaybackStatus::~SpotifyPlaybackStatus()
{}

QString SpotifyPlaybackStatus::deviceID() const
{
    return m_deviceID;
}

bool SpotifyPlaybackStatus::isPlaying() const
{
    return m_isPlaying;
}

int64_t SpotifyPlaybackStatus::progressMS() const
{
    return m_progressMS;
}

void SpotifyPlaybackStatus::setDeviceID(const QString& deviceID)
{
    m_deviceID = deviceID;
}

QString SpotifyPlaybackStatus::albumName() const
{
    return m_albumName;
}

QString SpotifyPlaybackStatus::artistsNames() const
{
    return m_artistsNames;
}

QString SpotifyPlaybackStatus::trackID() const
{
    return m_trackID;
}

QString SpotifyPlaybackStatus::trackHref() const
{
    return m_trackHref;
}

QString SpotifyPlaybackStatus::trackUri() const
{
    return m_trackUri;
}

QString SpotifyPlaybackStatus::trackName() const
{
    return m_trackName;
}

int64_t SpotifyPlaybackStatus::trackDurationMS() const
{
    return m_trackDurationMS;
}

void SpotifyPlaybackStatus::enablePlaybackWatching(bool enable)
{
    if (enable)
    {
        m_fetchStatusTimer->start(_timerInterval);
    }
    else 
    {
        m_fetchStatusTimer->stop();
    }
}

void SpotifyPlaybackStatus::setIsPlaying(bool isPlaying)
{
    if (isPlaying == m_isPlaying) return;
    m_isPlaying = isPlaying;
    emit isPlayingChanged();
}

void SpotifyPlaybackStatus::setProgressMS(int64_t progressMS)
{
    if (progressMS == m_progressMS) return;
    m_progressMS = progressMS;
    emit progressMSChanged();
}

void SpotifyPlaybackStatus::setAlbumName(const QString& albumName)
{
    if (albumName == m_albumName) return;
    m_albumName = albumName;
    emit albumNameChanged();
}

void SpotifyPlaybackStatus::setArtistsNames(const QString& artistsNames)
{
    if (artistsNames == m_artistsNames) return;
    m_artistsNames = artistsNames;
    emit artistsNamesChanged();
}

void SpotifyPlaybackStatus::setTrackID(const QString& trackID)
{
    if (trackID == m_trackID) return;
    m_trackID = trackID;
    emit trackIDChanged();
}

void SpotifyPlaybackStatus::setTrackHref(const QString& trackHref)
{
    if (trackHref == m_trackHref) return;
    m_trackHref = trackHref;
    emit trackHrefChanged();
}

void SpotifyPlaybackStatus::setTrackUri(const QString& trackUri)
{
    if (trackUri == m_trackUri) return;
    m_trackUri = trackUri;
    emit trackUriChanged();
}

void SpotifyPlaybackStatus::setTrackName(const QString& name)
{
    if (name == m_trackName) return;
    m_trackName = name;
    emit trackNameChanged();
}

void SpotifyPlaybackStatus::setTrackDurationMS(int64_t durationMS)
{
    if (durationMS == m_trackDurationMS) return;
    m_trackDurationMS = durationMS;
    emit trackDurationMSChanged();
}

void SpotifyPlaybackStatus::fetchStatus()
{
    if (!m_spotifyAuth) return;

    QUrlQuery urlQuery;
    urlQuery.addQueryItem("additional_types", "track");

    const QNetworkRequest request(_packbackStateEndpointUrl + "?" +
            urlQuery.query(QUrl::FullyEncoded));
    
    QNetworkReply* reply = m_spotifyAuth->get(request);
    connect(reply, &QNetworkReply::finished, 
            [this, reply]() {
        this->handleFetchStatusResponse(reply);
    });
}

void SpotifyPlaybackStatus::handleFetchStatusResponse(QNetworkReply* reply) 
{
    const QByteArray responseData = reply->readAll();
    reply->deleteLater();

    const QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
    
    if (!jsonDocument.isObject()) return;

    const QJsonObject rootObject = jsonDocument.object();

    if (!rootObject.contains("device") || !rootObject.contains("repeat_state") ||
        !rootObject.contains("shuffle_state") || !rootObject.contains("progress_ms") ||
        !rootObject.contains("is_playing") || !rootObject.contains("item") ||
        !rootObject.contains("currently_playing_type"))
    {
        return;
    }

    if (!parseDeviceInfo(rootObject.value("device"))) return;
    if (!parsePlaybackStatus(rootObject)) return;
    if (!parseCurrentTrack(rootObject)) return;
}

bool SpotifyPlaybackStatus::parseDeviceInfo(const QJsonValue& value) 
{
    if (!value.isObject()) return false;

    const QJsonObject deviceObject = value.toObject();

    if (deviceObject.isEmpty() || !deviceObject.contains("id"))
    {
        return false;
    }

    const QString id = deviceObject.value("id").toString();

    if (id.isEmpty()) return false;

    setDeviceID(id);
    return true;
}

bool SpotifyPlaybackStatus::parsePlaybackStatus(const QJsonObject& rootObject)
{
    const QString repeatState = rootObject.value("repeat_state").toString();
    const bool suffleState = rootObject.value("shuffle_state").toBool();
    const int64_t progressMS = rootObject.value("progress_ms").toInteger();
    const bool isPlaying = rootObject.value("is_playing").toBool();

    setProgressMS(progressMS);
    setIsPlaying(isPlaying);
    return true;
}

bool SpotifyPlaybackStatus::parseCurrentTrack(const QJsonObject& rootObject)
{
    const QString currentlyPlayingType = 
        rootObject.value("currently_playing_type").toString();

    if (currentlyPlayingType != "track") return false;

    const QJsonValue itemValue = rootObject.value("item");

    if (!itemValue.isObject()) return false;

    const QJsonObject itemObject = itemValue.toObject();

    if (itemObject.isEmpty()) return false;

    bool isError = false;

    const SpotifyReceivedPlaylistElement::Track track =
        SpotifyReceivedPlaylistElement::parseInnerTrack(itemObject, &isError);

    if (isError) return false;

    setAlbumName(track.album);
    setArtistsNames(track.artists);
    setTrackID(track.id);
    setTrackHref(track.href);
    setTrackUri(track.uri);
    setTrackName(track.name);
    setTrackDurationMS(track.durationMS);

    return true;
}

void SpotifyPlaybackStatus::playbackStartPlaying()
{
    setIsPlaying(true);
}

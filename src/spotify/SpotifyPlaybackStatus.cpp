#include "spotify/SpotifyPlaybackStatus.h"
#include "SpotifyAuthorizationPKCE.h"
#include "SpotifyPlayer.h"
#include "SpotifyPlaylist.h"
#include "SpotifyReceivedPlaylistElement.h"
#include <bits/chrono.h>
#include <chrono>
#include <cstdint>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkreply.h>
#include <qurlquery.h>

int SpotifyPlaybackStatus::_timerInterval = 2500;
int SpotifyPlaybackStatus::_progressTimerInterval = 1000;
const QString SpotifyPlaybackStatus::_packbackStateEndpointUrl =
    "https://api.spotify.com/v1/me/player";

SpotifyPlaybackStatus::SpotifyPlaybackStatus(
        SpotifyAuthorizationPKCE* spotifyAuth, 
        SpotifyPlayer* player, QObject* parent) :

    QObject(parent),
    m_fetchStatusTimer(new QTimer(this)),
    m_progressTimer(new QTimer(this)),
    m_spotifyAuth(spotifyAuth),
    m_spotifyPlayer(player),
    m_isPlaying(false),
    m_shuffleState(false),
    m_progressMS(0),
    m_trackDurationMS(0),
    m_fetchProgressMS(0),
    m_isFetchProgressUpdated(false)
{
    connect(m_fetchStatusTimer, &QTimer::timeout, this, &SpotifyPlaybackStatus::fetchStatus);
    connect(m_progressTimer, &QTimer::timeout, this, &SpotifyPlaybackStatus::incrementProgress);
    connect(this, &SpotifyPlaybackStatus::isPlayingChanged, this, &SpotifyPlaybackStatus::startProgressTimer);
    connect(m_spotifyPlayer, &SpotifyPlayer::isPlaying, this, &SpotifyPlaybackStatus::playbackStartPlaying);
    connect(m_spotifyPlayer, &SpotifyPlayer::isResuming, this, &SpotifyPlaybackStatus::playbackStartResuming);
    connect(m_spotifyPlayer, &SpotifyPlayer::isPausing, this, &SpotifyPlaybackStatus::playbackStartPausing);
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

bool SpotifyPlaybackStatus::shuffleState() const 
{
    return m_shuffleState;
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

QUrl SpotifyPlaybackStatus::albumImage() const
{
    return m_albumImage;
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
        resetInfos();
    }
}

void SpotifyPlaybackStatus::setIsPlaying(bool isPlaying)
{
    if (isPlaying == m_isPlaying) return;
    m_isPlaying = isPlaying;
    emit isPlayingChanged();
}

void SpotifyPlaybackStatus::setShuffleState(bool shuffleState) 
{
    if (shuffleState == m_shuffleState) return;
    m_shuffleState = shuffleState;
    emit shuffleStateChanged();
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

void SpotifyPlaybackStatus::setAlbumImage(const QUrl& albumImage)
{
    if (albumImage == m_albumImage) return;

    m_albumImage = albumImage;
    emit albumImageChanged();
}

void SpotifyPlaybackStatus::setTrackDurationMS(int64_t durationMS)
{
    if (durationMS == m_trackDurationMS) return;
    m_trackDurationMS = durationMS;
    emit trackDurationMSChanged();
}

void SpotifyPlaybackStatus::setFetchProgressMS(int64_t progressMS)
{
    m_fetchProgressMS = progressMS;
    m_isFetchProgressUpdated = true;
    m_fetchProgressLastSet = std::chrono::system_clock::now();
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
    if (!parseCurrentTrack(rootObject)) return;
    if (!parsePlaybackStatus(rootObject)) return;
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
    const bool shuffleState = rootObject.value("shuffle_state").toBool();
    const int64_t progressMS = rootObject.value("progress_ms").toInteger();
    const bool isPlaying = rootObject.value("is_playing").toBool();

    setFetchProgressMS(progressMS);
    setIsPlaying(isPlaying);
    setShuffleState(shuffleState);
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

    const QUrl albumImage = getAlbumImage(rootObject);

    if (isError) return false;

    setTrackID(track.id);
    setTrackHref(track.href);
    setTrackUri(track.uri);
    setAlbumName(track.album);
    setArtistsNames(track.artists);
    setTrackName(track.name);
    setAlbumImage(albumImage);
    setTrackDurationMS(track.durationMS);

    return true;
}

QUrl SpotifyPlaybackStatus::getAlbumImage(const QJsonObject& rootObject)
{
    if (!rootObject.contains("item")) return QUrl();

    const QJsonObject& itemObject = rootObject.value("item").toObject();

    if (!itemObject.contains("album")) return QUrl();

    const QJsonObject& albumObject = itemObject.value("album").toObject();

    if (!albumObject.contains("images")) return QUrl();

    const QJsonValue& imagesValue = albumObject.value("images");

    const QUrl imageUrl = SpotifyPlaylist::parseImagesUrls(imagesValue);
    return imageUrl;
}

void SpotifyPlaybackStatus::playbackStartPlaying()
{
    enablePlaybackWatching(true);
    setIsPlaying(true);
}

void SpotifyPlaybackStatus::playbackStartResuming()
{
    setIsPlaying(true);
}

void SpotifyPlaybackStatus::playbackStartPausing()
{
    setIsPlaying(false);
}

void SpotifyPlaybackStatus::resetInfos()
{
    setIsPlaying(false);
    setProgressMS(0);
    setAlbumName(QString());
    setArtistsNames(QString());
    setTrackID(QString());
    setTrackUri(QString());
    setTrackName(QString());
    setTrackHref(QString());
    setAlbumImage(QUrl());
    setTrackDurationMS(0);
}

void SpotifyPlaybackStatus::incrementProgress()
{
    const auto currentTime = std::chrono::system_clock::now();
    const int64_t incrementSinceLastTime = 
        std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_progressTimerLastCall).count();
    m_progressTimerLastCall = std::chrono::system_clock::now();

    if (m_isFetchProgressUpdated)
    {
        const int64_t incrementSinceFetch = 
            std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_fetchProgressLastSet).count();
        setProgressMS(m_fetchProgressMS + incrementSinceFetch);
        m_isFetchProgressUpdated = false;
        return;
    }

    setProgressMS(m_progressMS + incrementSinceLastTime);
}

void SpotifyPlaybackStatus::startProgressTimer()
{
    if (m_isPlaying)
    {
        m_progressTimer->start(_progressTimerInterval);
        m_progressTimerLastCall = std::chrono::system_clock::now();
    }
    else 
    {
        m_progressTimer->stop();
    }
}

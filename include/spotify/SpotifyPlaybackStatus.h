#ifndef SIMPLEAUDIOPLAYER_SPOTIFY_PLAYBACK_STATUS_H_
#define SIMPLEAUDIOPLAYER_SPOTIFY_PLAYBACK_STATUS_H_

#include "SpotifyAuthorizationPKCE.h"
#include "SpotifyPlayer.h"
#include "SpotifyUserInfo.h"
#include <cstdint>
#include <qobject.h>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <sys/types.h>
#include <chrono>

class SpotifyPlaybackStatus : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(int64_t progressMS READ progressMS NOTIFY progressMSChanged)
    Q_PROPERTY(bool shuffleState READ shuffleState NOTIFY shuffleStateChanged)

    Q_PROPERTY(QString albumName READ albumName NOTIFY albumNameChanged)
    Q_PROPERTY(QString albumID READ albumID NOTIFY albumIDChanged)
    Q_PROPERTY(QString artistsNames READ artistsNames NOTIFY artistsNamesChanged)
    Q_PROPERTY(QString trackID READ trackID NOTIFY trackIDChanged)
    Q_PROPERTY(QString trackUri READ trackUri NOTIFY trackUriChanged)
    Q_PROPERTY(QString trackName READ trackName NOTIFY trackNameChanged)
    Q_PROPERTY(QString trackHref READ trackHref NOTIFY trackHrefChanged)
    Q_PROPERTY(QUrl albumImage READ albumImage NOTIFY albumImageChanged)
    Q_PROPERTY(int64_t trackDurationMS READ trackDurationMS NOTIFY trackDurationMSChanged)

public:
    SpotifyPlaybackStatus(SpotifyAuthorizationPKCE* spotifyAuth, 
            SpotifyPlayer* player, 
            SpotifyUserInfo* userInfo,
            QObject* parent = nullptr
        );
    virtual ~SpotifyPlaybackStatus();

    QString deviceID() const;

    bool isPlaying() const;
    int64_t progressMS() const;
    bool shuffleState() const;

    QString albumName() const;
    QString albumID() const;
    QString artistsNames() const;
    QString trackID() const;
    QString trackUri() const;
    QString trackName() const;
    QString trackHref() const;
    QUrl albumImage() const;
    int64_t trackDurationMS() const;

public slots:
    void setDeviceID(const QString& deviceID);
    void enablePlaybackWatching(bool enable);

signals:
    void isPlayingChanged();
    void progressMSChanged();
    void shuffleStateChanged();

    void albumNameChanged();
    void albumIDChanged();
    void artistsNamesChanged();
    void trackIDChanged();
    void trackUriChanged();
    void trackNameChanged();
    void trackHrefChanged();
    void albumImageChanged();
    void trackDurationMSChanged();

private:
    void setIsPlaying(bool isPlaying);
    void setProgressMS(int64_t progressMS);
    void setShuffleState(bool state);

    void setAlbumName(const QString& albumName);
    void setAlbumID(const QString& id);
    void setArtistsNames(const QString& artistsNames);
    void setTrackID(const QString& trackID);
    void setTrackUri(const QString& trackUri);
    void setTrackName(const QString& trackName);
    void setTrackHref(const QString& trackHref);
    void setAlbumImage(const QUrl& albumImage);
    void setTrackDurationMS(int64_t durationMS);
    void setFetchProgressMS(int64_t progressMS);

    void fetchStatus();
    void handleFetchStatusResponse(QNetworkReply* reply);

    void incrementProgress();
    void startProgressTimer();

    bool parseDeviceInfo(const QJsonValue& value);
    bool parsePlaybackStatus(const QJsonObject& rootObject);
    bool parseCurrentTrack(const QJsonObject& rootObject);
    QUrl getAlbumImage(const QJsonObject& rootObject);

    void playbackStartPlaying();
    void playbackStartResuming();
    void playbackStartPausing();

    void resetInfos();

    QTimer* m_fetchStatusTimer;
    SpotifyAuthorizationPKCE* m_spotifyAuth;
    SpotifyPlayer* m_spotifyPlayer;
    SpotifyUserInfo* m_userInfo;

    QString m_deviceID;
    bool m_isPlaying;
    bool m_shuffleState;
    int64_t m_progressMS;

    QString m_albumName;
    QString m_albumID;
    QString m_artistsNames;
    QString m_trackID;
    QString m_trackUri;
    QString m_trackName;
    QString m_trackHref;
    QUrl m_albumImage;
    int64_t m_trackDurationMS;

    int64_t m_fetchProgressMS;
    bool m_isFetchProgressUpdated;
    std::chrono::system_clock::time_point m_fetchProgressLastSet;

    QTimer* m_progressTimer;
    std::chrono::system_clock::time_point m_progressTimerLastCall;

    static int _timerInterval;
    static int _progressTimerInterval;
    static const QString _packbackStateEndpointUrl;
};

#endif //SIMPLEAUDIOPLAYER_SPOTIFY_PLAYBACK_STATUS_H_

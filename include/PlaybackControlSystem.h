#ifndef SIMPLEAUDIOPLAYER_PLAYBACK_CONTROL_SYSTEM_H_
#define SIMPLEAUDIOPLAYER_PLAYBACK_CONTROL_SYSTEM_H_

#include "Player.h"
#include "spotify/SpotifyAPI.h"
#include <qobject.h>
#include <qqmlintegration.h>

class PlaybackControlSystem : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Player* salPlayer READ salPlayer WRITE setSalPlayer NOTIFY salPlayerChanged)
    Q_PROPERTY(SpotifyAPI* spotifyAPI READ spotifyAPI WRITE setSpotifyAPI NOTIFY spotifyAPIChanged)

    Q_PROPERTY(StreamBackend currentBackend READ currentBackend WRITE setCurrentBackend NOTIFY currentBackendChanged)

    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)
    Q_PROPERTY(bool isReady READ isReady NOTIFY isReadyChanged)
    Q_PROPERTY(bool isShuffled READ isShuffled WRITE setIsShuffledFromQML NOTIFY isShuffledChanged)
    Q_PROPERTY(long long streamSize READ streamSize NOTIFY streamSizeChanged)
    Q_PROPERTY(long long streamSizeSeconds READ streamSizeSeconds NOTIFY streamSizeSecondsChanged)
    Q_PROPERTY(long long streamPos READ streamPos NOTIFY setStreamPos NOTIFY streamPosChanged)
    Q_PROPERTY(long long streamPosSeconds READ streamPosSeconds NOTIFY streamPosSecondsChanged)
    Q_PROPERTY(QStringList supportedFormats READ supportedFormats NOTIFY supportedFormatsChanged)
    Q_PROPERTY(QString currentStream READ currentStream NOTIFY currentStreamChanged)
    Q_PROPERTY(QString trackName READ spotifyTrackName NOTIFY spotifyTrackNameChanged)
    Q_PROPERTY(QString trackArtists READ spotifyTrackArtists NOTIFY spotifyTrackArtistsChanged)
    Q_PROPERTY(QString trackAlbumName READ spotifyTrackAlbumName NOTIFY spotifyTrackAlbumNameChanged)
    Q_PROPERTY(QString trackAlbumCover READ spotifyAlbumCover NOTIFY spotifyAlbumCoverChanged)

public:
    enum class StreamBackend
    {
        SAL,
        SPOTIFY
    };
    Q_ENUM(StreamBackend)

    PlaybackControlSystem();
    virtual ~PlaybackControlSystem();

    Player* salPlayer();
    SpotifyAPI* spotifyAPI();

    StreamBackend currentBackend() const;

    bool isRunning() const;
    bool isPlaying() const;
    bool isPaused() const;
    bool isReady() const;
    bool isShuffled() const;
    long long streamSize() const;
    long long streamSizeSeconds() const;
    long long streamPos() const;
    long long streamPosSeconds() const;
    QStringList supportedFormats() const;
    QString currentStream() const;

    QString spotifyTrackName() const;
    QString spotifyTrackArtists() const;
    QString spotifyTrackAlbumName() const;
    QString spotifyAlbumCover() const;

public slots:
    void setSalPlayer(Player* player);
    void setSpotifyAPI(SpotifyAPI* spotifyAPI);

    void setCurrentBackend(StreamBackend backend);

    bool isReadable(const QString& filePath) const;
    void open(const QString& filePath);
    void open(const QStringList& filesPath, bool isShuffledChanged = false);

    void playPause();
    void play();
    void pause();
    void stop();
    void seek(long long pos);
    void next();
    void previous();

signals:
    void salPlayerChanged();
    void spotifyAPIChanged();

    void currentBackendChanged();

    void isRunningChanged();
    void isPlayingChanged();
    void isPausedChanged();
    void isReadyChanged();
    void isShuffledChanged();
    void streamSizeChanged();
    void streamSizeSecondsChanged();
    void streamPosChanged();
    void streamPosSecondsChanged();
    void supportedFormatsChanged(); // remove warning
    void currentStreamChanged();

    void spotifyTrackNameChanged();
    void spotifyTrackArtistsChanged();
    void spotifyTrackAlbumNameChanged();
    void spotifyAlbumCoverChanged();

private:
    void setIsRunning(bool isRunning);
    void setIsPlaying(bool isPlaying);
    void setIsPaused(bool isPaused);
    void setIsReady(bool isReady);
    void setIsShuffled(bool isShuffled);
    void setIsShuffledFromQML(bool isShuffled);
    void setStreamSize(long long streamSize);
    void setStreamSizeSeconds(long long streamSizeSeconds);
    void setStreamPos(long long streamPos);
    void setStreamPosSeconds(long long streamPosSeconds);
    void setCurrentStream(const QString& currentStream);

    void setSpotifyTrackName(const QString& trackName);
    void setSpotifyTrackArtists(const QString& trackArtists);
    void setSpotifyTrackAlbumName(const QString& trackAlbum);
    void setSpotifyAlbumCover(const QString& albumCover);

    void setSignalsOfSalPlayer();
    void setSignalsOfSpotifyAPI();
    void handleStartNewFileSignal(const QString& filePath);
    void handleEndFileSignal(const QString& filePath);
    void handleStreamPosChangedInFrames(long long frames);
    void handleStreamPosChanged(long long streamPos);
    void handleStreamPausedSignal();
    void handleStreamPlaying();
    void handleStreamStopping();
    void handleCurrentStreamChanged();
    void handleIsReadySignalChanged(bool isReady);

    void handleSpotifyTrackURIChanged();
    void handleSpotifyTrackDurationChange();
    void handleSpotifyTrackProgressChange();
    void handleSpotifyTrackNameChange();
    void handleSpotifyTrackAlbumChange();
    void handleSpotifyTrackArtistsChange();
    void handleSpotifyTrackAlbumCoverChange();
    void handleSpotifyShuffleStateChange();

    void handleSpotifyIsPlayingStatusChange();

    bool isSal() const;
    bool isSpotify() const;

    void reenableSAL();

    Player* m_salPlayer;
    SpotifyAPI* m_spotifyAPI;

    StreamBackend m_currentBackend;

    bool m_isRunning;
    bool m_isPlaying;
    bool m_isPaused;
    bool m_isReady;
    bool m_isShuffled;
    long long m_streamSize;
    long long m_streamSizeSeconds;
    long long m_streamPos;
    long long m_streamPosSeconds;
    long long m_streamPosUpdatingBuffer;
    QString m_currentStream;

    QString m_spotifyTrackName;
    QString m_spotifyTrackArtists;
    QString m_spotifyTrackAlbumName;
    QString m_spotifyTrackAlbumCover;
};

#endif // SIMPLEAUDIOPLAYER_PLAYBACK_CONTROL_SYSTEM_H_

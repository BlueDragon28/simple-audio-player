#include "SpotifyAPI.h"
#include <PlaybackControlSystem.h>
#include <cstdint>
#include <spotify/SpotifyPlaybackStatus.h>

PlaybackControlSystem::PlaybackControlSystem() :
    QObject(),
    m_salPlayer(nullptr),
    m_spotifyAPI(nullptr),

    m_currentBackend(StreamBackend::SAL),

    m_isRunning(false),
    m_isPlaying(false),
    m_isPaused(false),
    m_isReady(false),
    m_streamSize(0),
    m_streamSizeSeconds(0),
    m_streamPos(0),
    m_streamPosSeconds(0),
    m_streamPosUpdatingBuffer(0)
{}

PlaybackControlSystem::~PlaybackControlSystem()
{}

Player* PlaybackControlSystem::salPlayer()
{
    return m_salPlayer;
}

SpotifyAPI* PlaybackControlSystem::spotifyAPI()
{
    return m_spotifyAPI;
}

PlaybackControlSystem::StreamBackend PlaybackControlSystem::currentBackend() const
{
    return m_currentBackend;
}

bool PlaybackControlSystem::isRunning() const
{
    return m_isRunning;
}

bool PlaybackControlSystem::isPlaying() const
{
    return m_isPlaying;
}

bool PlaybackControlSystem::isPaused() const
{
    return m_isPaused;
}

bool PlaybackControlSystem::isReady() const
{
    return m_isReady;
}

long long PlaybackControlSystem::streamSize() const
{
    return m_streamSize;
}

long long PlaybackControlSystem::streamSizeSeconds() const
{
    return m_streamSizeSeconds;
}

long long PlaybackControlSystem::streamPos() const 
{
    return m_streamPos;
}

long long PlaybackControlSystem::streamPosSeconds() const
{
    return m_streamPosSeconds;
}

QStringList PlaybackControlSystem::supportedFormats() const
{
    if (!m_salPlayer) return {};

    return m_salPlayer->supportedFormats();
}

QString PlaybackControlSystem::currentStream() const
{
    return m_currentStream;
}

void PlaybackControlSystem::setSalPlayer(Player* player)
{
    if (player == m_salPlayer) return;
    m_salPlayer = player;
    emit salPlayerChanged();
    setSignalsOfSalPlayer();
}

void PlaybackControlSystem::setSpotifyAPI(SpotifyAPI* spotifyAPI)
{
    if (spotifyAPI == m_spotifyAPI) return;

    m_spotifyAPI = spotifyAPI;
    emit spotifyAPIChanged();
    setSignalsOfSpotifyAPI();
}

void PlaybackControlSystem::setCurrentBackend(StreamBackend backend)
{
    if (backend == m_currentBackend) return;
    m_currentBackend = backend;
    emit currentBackendChanged();
}

void PlaybackControlSystem::setIsRunning(bool isRunning)
{
    if (isRunning == m_isRunning) return;
    m_isRunning = isRunning;
    emit isRunningChanged();
}

void PlaybackControlSystem::setIsPlaying(bool isPlaying)
{
    if (isPlaying == m_isPlaying) return;
    m_isPlaying = isPlaying;
    emit isPlayingChanged();
}

void PlaybackControlSystem::setIsPaused(bool isPaused)
{
    if (isPaused == m_isPaused) return;
    m_isPaused = isPaused;
    emit isPausedChanged();
}

void PlaybackControlSystem::setIsReady(bool isReady)
{
    if (isReady == m_isReady) return;
    m_isReady = isReady;
    emit isReadyChanged();
}

void PlaybackControlSystem::setStreamSize(long long streamSize)
{
    if (streamSize == m_streamSize) return;
    m_streamSize = streamSize;
    emit streamSizeChanged();
}

void PlaybackControlSystem::setStreamSizeSeconds(long long streamSizeSeconds)
{
    if (streamSizeSeconds == m_streamSize) return;
    m_streamSizeSeconds = streamSizeSeconds;
    emit streamSizeSecondsChanged();
}

void PlaybackControlSystem::setStreamPos(long long streamPos)
{
    if (streamPos == m_streamPos) return;
    m_streamPos = streamPos;
    emit streamPosChanged();
}

void PlaybackControlSystem::setStreamPosSeconds(long long streamPosSeconds)
{
    if (streamPosSeconds == m_streamPosSeconds) return;
    m_streamPosSeconds = streamPosSeconds;
    emit streamPosSecondsChanged();
}

void PlaybackControlSystem::setCurrentStream(const QString& currentStream)
{
    if (currentStream == m_currentStream) return;
    m_currentStream = currentStream;
    emit currentStreamChanged();
}

void PlaybackControlSystem::setSignalsOfSalPlayer()
{
    if (!m_salPlayer) return;

    connect(m_salPlayer, &Player::startNewFile, this, &PlaybackControlSystem::handleStartNewFileSignal);
    connect(m_salPlayer, &Player::endFile, this, &PlaybackControlSystem::handleEndFileSignal);
    connect(m_salPlayer, &Player::streamPosChangeInFrames, this, &PlaybackControlSystem::handleStreamPosChangedInFrames);
    connect(m_salPlayer, &Player::streamPosChangeInSeconds, this, &PlaybackControlSystem::handleStreamPosChanged);
    connect(m_salPlayer, &Player::streamPaused, this, &PlaybackControlSystem::handleStreamPausedSignal);
    connect(m_salPlayer, &Player::streamPlaying, this, &PlaybackControlSystem::handleStreamPlaying);
    connect(m_salPlayer, &Player::streamStopping, this, &PlaybackControlSystem::handleStreamStopping);
    connect(m_salPlayer, &Player::currentStreamChanged, this, &PlaybackControlSystem::handleCurrentStreamChanged);
    connect(m_salPlayer, &Player::isReadyChanged, this, &PlaybackControlSystem::handleIsReadySignalChanged);
}

void PlaybackControlSystem::setSignalsOfSpotifyAPI()
{
    if (!m_spotifyAPI) return;

    SpotifyPlaybackStatus* playbackStatus = m_spotifyAPI->playbackStatus();
    connect(playbackStatus, &SpotifyPlaybackStatus::isPlayingChanged, this, &PlaybackControlSystem::handleSpotifyIsPlayingStatusChange);
    connect(playbackStatus, &SpotifyPlaybackStatus::trackUriChanged, this, &PlaybackControlSystem::handleSpotifyTrackURIChanged);
    connect(playbackStatus, &SpotifyPlaybackStatus::trackDurationMSChanged, this, &PlaybackControlSystem::handleSpotifyTrackDurationChange);
    connect(playbackStatus, &SpotifyPlaybackStatus::progressMSChanged, this, &PlaybackControlSystem::handleSpotifyTrackProgressChange);
}

void PlaybackControlSystem::handleSpotifyIsPlayingStatusChange()
{
    if (!m_spotifyAPI || !m_spotifyAPI->isAuthenticated()) return;

    if (m_currentBackend == StreamBackend::SAL)
    {
        if (!m_salPlayer) return;

        m_salPlayer->stop();
        setCurrentBackend(StreamBackend::SPOTIFY);
    }

    const bool isPlaying = m_spotifyAPI->playbackStatus()->isPlaying();
    setIsReady(true);
    setIsPlaying(isPlaying);
    m_spotifyAPI->playbackStatus()->enablePlaybackWatching(true);
}

bool PlaybackControlSystem::isSal() const
{
    return m_salPlayer && m_currentBackend == StreamBackend::SAL;
}

bool PlaybackControlSystem::isSpotify() const
{
    return m_spotifyAPI && m_currentBackend == StreamBackend::SPOTIFY;
}

void PlaybackControlSystem::handleStartNewFileSignal(const QString& filePath)
{
    if (!isSal()) return;
    setCurrentStream(filePath);
    setStreamSize(m_salPlayer->streamSize());
    setStreamSizeSeconds(m_salPlayer->streamSize(Player::SECONDS));
}

void PlaybackControlSystem::handleEndFileSignal(const QString& filePath)
{
    if (!isSal()) return;
    setCurrentStream(QString());
}

void PlaybackControlSystem::handleStreamPosChangedInFrames(long long frames)
{
    if (!isSal()) return;
    m_streamPosUpdatingBuffer = frames;
}

void PlaybackControlSystem::handleStreamPosChanged(long long streamPos)
{
    if (!isSal()) return;
    setStreamPos(m_streamPosUpdatingBuffer);
    setStreamPosSeconds(streamPos);
}

void PlaybackControlSystem::handleStreamPausedSignal()
{
    if (!isSal()) return;
    setIsPlaying(true);
    setIsPaused(true);
}

void PlaybackControlSystem::handleStreamPlaying()
{
    if (!isSal()) return;
    setIsPlaying(true);
    setIsPaused(false);
}

void PlaybackControlSystem::handleStreamStopping()
{
    if (!isSal()) return;
    setIsPaused(false);
    setIsPlaying(false);
}

void PlaybackControlSystem::handleCurrentStreamChanged()
{
    if (!isSal()) return;
    setCurrentStream(m_salPlayer->currentStream());
}

void PlaybackControlSystem::handleIsReadySignalChanged(bool isReady)
{
    if (!isSal()) return;
    setIsReady(isReady);
}

void PlaybackControlSystem::handleSpotifyTrackURIChanged()
{
    if (!isSpotify()) return;

    setCurrentStream(m_spotifyAPI->playbackStatus()->trackUri());
}

void PlaybackControlSystem::handleSpotifyTrackDurationChange()
{
    if (!isSpotify()) return;

    const int64_t durationMS = m_spotifyAPI->playbackStatus()->trackDurationMS();
    setStreamSize(durationMS);
    setStreamSizeSeconds(durationMS / 1000);
}

void PlaybackControlSystem::handleSpotifyTrackProgressChange()
{
    if (!isSpotify()) return;

    const int64_t positionMS = m_spotifyAPI->playbackStatus()->progressMS();
    setStreamPos(positionMS);
    setStreamPosSeconds(positionMS / 1000);
}

bool PlaybackControlSystem::isReadable(const QString& filePath) const
{
    if (!isSal()) return false;
    return m_salPlayer->isReadable(filePath);
}

void PlaybackControlSystem::open(const QString& filePath)
{
    if (!isSal()) return;
    m_salPlayer->open(filePath);
}

void PlaybackControlSystem::open(const QStringList& filesPath, bool isShuffledChanged)
{
    if (!isSal()) return;
    m_salPlayer->open(filesPath, isShuffledChanged);
}

void PlaybackControlSystem::play()
{
    if (isSal())
    {
        m_salPlayer->play();
    }
}

void PlaybackControlSystem::pause()
{
    if (isSal())
    {
        m_salPlayer->pause();
    }
}

void PlaybackControlSystem::stop()
{
    if (isSal())
    {
        m_salPlayer->stop();
    }
}

void PlaybackControlSystem::seek(long long pos)
{
    if (isSal() && pos >= 0)
    {
        m_salPlayer->seek(pos);
    }
}

void PlaybackControlSystem::next()
{
    if (isSal())
    {
        m_salPlayer->next();
    }
}

void PlaybackControlSystem::previous()
{
    if (isSal())
    {
        seek(0);
    }
}

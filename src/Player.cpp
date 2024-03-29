#include "Player.h"
#include "AppConfig.h"
#include <simple-audio-library/AudioPlayer.h>
#include <QDebug>
#include <QStandardPaths>

Player* Player::m_instance = nullptr;

Player::Player() :
    QObject(nullptr),
    m_player(SAL::AudioPlayer::instance())
{
    // Bind the event wrapper to the SAL instance.
    m_player->callback().addStartFileCallback(std::bind(&Player::salStartNewFile, this, std::placeholders::_1));
    m_player->callback().addEndFileCallback(std::bind(&Player::salEndFile, this, std::placeholders::_1));
    m_player->callback().addStreamPosChangeCallback(std::bind(&Player::salStreamPosChangeInSeconds, this, std::placeholders::_1), SAL::TimeType::SECONDS);
    m_player->callback().addStreamPosChangeCallback(std::bind(&Player::salStreamPosChangeInFrames, this, std::placeholders::_1), SAL::TimeType::FRAMES);
    m_player->callback().addStreamPausedCallback(std::bind(&Player::salStreamPaused, this));
    m_player->callback().addStreamPlayingCallback(std::bind(&Player::salStreamPlaying, this));
    m_player->callback().addStreamStoppingCallback(std::bind(&Player::salStreamStopping, this));
    m_player->callback().addIsReadyChangedCallback(std::bind(&Player::salIsReadyChanged, this, std::placeholders::_1));

    // Apply backend audio.
    m_player->setBackendAudio(AppConfig::getBackendAudioSetting());

    // Create the debug log file.
    QStringList strList = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    if (!strList.isEmpty())
    {
#ifdef DEBUG_LOG
        SAL::DebugLog::instance()->setFilePath((strList.at(0) + "/simple-audio-player.log").toStdString());
#endif
    }

    Player::m_instance = this;
}

Player::~Player()
{
    // Destroy the simple-audio-library instance.
    SAL::AudioPlayer::deinit();

    Player::m_instance = nullptr;
}

/*
Return true if simple-audio-library is initialized.
*/
bool Player::isRunning() const
{
    return m_player->isRunning();
}

/*
Return true if simple-audio-library is playing an audio file.
*/
bool Player::isPlaying() const
{
    return m_player->isPlaying();
}

/*
Is simple-audio-library is ready to play (or is playing) files.
*/
bool Player::isReady() const
{
    return m_player->isReady();
}

/*
Return current stream size.
*/
size_t Player::streamSize(SizeType type) const
{
    if (type == FRAMES)
        return m_player->streamSize(SAL::TimeType::FRAMES);
    else
        return m_player->streamSize(SAL::TimeType::SECONDS);
}

/*
Return current stream position.
*/
size_t Player::streamPos(SizeType type) const
{
    if (type == FRAMES)
        return m_player->streamPos(SAL::TimeType::FRAMES);
    else
        return m_player->streamPos(SAL::TimeType::SECONDS);
}

/*
Check if a file is readable by the simple-audio-library.
*/
int Player::isReadable(const QString& filePath) const
{
    return m_player->isReadable(filePath.toStdString());
}

/*
Return a list of strings of all the available files formats readable by the simple-audio-library.
*/
QList<QString> Player::supportedFormats() const
{
    return {};
}

/*
Open an audio file.
*/
void Player::open(const QString& filePath)
{
#ifdef WIN32
    m_player->open(filePath.toUtf8().constData(), true);
#else
    m_player->open(filePath.toStdString(), true);
#endif
}

void Player::open(const QStringList& filesPath, bool isShuffledChanged)
{
    bool clearQueue = true;

    // Making a copy of filesPath and do not include the first element if 
    QStringList copyFilesPath;

    int firstIndex = 0;

    if (isShuffledChanged)
    {
        m_player->keepOnlyCurrentPlayback();

        if (isPlaying() && filesPath.size() >= 2)
        {
            firstIndex = 1;
            clearQueue = false;
        }
        else 
        {
            return;
        }
    }

    copyFilesPath.append(
        filesPath.cbegin() + firstIndex,
        filesPath.cend());

    foreach (const QString& path, copyFilesPath)
    {
#ifdef WIN32
        m_player->open(path.toUtf8().constData(), clearQueue);
#else
        m_player->open(path.toStdString(), clearQueue);
#endif
        if (clearQueue)
            clearQueue = false;
    }
}

/*
Start playing or resuming the stream.
*/
void Player::play()
{
    m_player->play();
}
/*
Pause the stream.
*/
void Player::pause()
{
    m_player->pause();
}

/*
Stop the stream.
*/
void Player::stop()
{
    m_player->stop();
}

/*
Seek inside the stream to a position.
*/
void Player::seek(size_t pos, SizeType type)
{
    m_player->seek(pos, type == SECONDS ? true : false);
}

/*
Move to the next audio stream.
*/
void Player::next()
{
    m_player->next();
}

/*
This methods are wrapped between the event system of the simple-audio-libray and Qt.
*/
void Player::salStartNewFile(const std::string& filePath)
{
    QString qStrFilePath =
#ifdef WIN32
            QString::fromUtf8(filePath);
#else
            QString::fromStdString(filePath);
#endif
    emit startNewFile(qStrFilePath);
    // Set current stream played property.
    setCurrentStream(qStrFilePath);
}

void Player::salEndFile(const std::string& filePath)
{
    emit endFile(
#ifdef WIN32
                QString::fromUtf8(filePath));
#else
                QString::fromStdString(filePath));
#endif
    // Reset current stream played property.
    setCurrentStream("");
}

void Player::salStreamPosChangeInFrames(size_t streamPos)
{
    emit streamPosChangeInFrames(streamPos);
}

void Player::salStreamPosChangeInSeconds(size_t streamPos)
{
    emit streamPosChangeInSeconds(streamPos);
}

void Player::salStreamPaused()
{
    emit streamPaused();
}

void Player::salStreamPlaying()
{
    emit streamPlaying();
}

void Player::salStreamStopping()
{
    emit streamStopping();
}

void Player::salIsReadyChanged(bool isReady)
{
    emit isReadyChanged(isReady);
}

/*
Currently played audio file path.
*/
QString Player::currentStream() const
{
    return m_currentStream;
}

/*
Set the current stream played.
*/
void Player::setCurrentStream(const QString& filePath)
{
    if (filePath != m_currentStream)
    {
        m_currentStream = filePath;
        emit currentStreamChanged();
    }
}

Player* Player::getInstance()
{
    return m_instance;
}

SAL::AudioPlayer* Player::getPlayer() const
{
    return m_player;
}

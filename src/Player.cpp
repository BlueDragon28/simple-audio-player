#include "Player.h"
#include <QDebug>

Player* Player::staticInstance = nullptr;

Player::Player(QObject* parent) :
    QObject(parent),
    m_player(SAL::AudioPlayer::instance())
{
    // Bind the event wrapper to the SAL instance.
    m_player->callback().addStartFileCallback(std::bind(&Player::salStartNewFile, this, std::placeholders::_1));
    m_player->callback().addEndFileCallback(std::bind(&Player::salEndFile, this, std::placeholders::_1));
    m_player->callback().addStreamPosChangeCallback(std::bind(&Player::salStreamPosChangeInSeconds, this, std::placeholders::_1));
    m_player->callback().addStreamPosChangeInFramesCallback(std::bind(&Player::salStreamPosChangeInFrames, this, std::placeholders::_1));
    m_player->callback().addStreamPausedCallback(std::bind(&Player::salStreamPaused, this));
    m_player->callback().addStreamPlayingCallback(std::bind(&Player::salStreamPlaying, this));
    m_player->callback().addStreamStoppingCallback(std::bind(&Player::salStreamStopping, this));
    m_player->callback().addIsReadyChangedCallback(std::bind(&Player::salIsReadyChanged, this, std::placeholders::_1));
}

Player::~Player()
{
    // Destroy the simple-audio-library instance.
    SAL::AudioPlayer::deinit();
}

/*
Static function to instanciate the singleton.
*/
QObject* Player::qmlRegistration(QQmlEngine* qmlEngine, QJSEngine* jsEngine)
{
    staticInstance = new Player();
    return staticInstance;
}

/*
Static function to retrieve the singleton instance.
*/
Player* Player::instance()
{
    return staticInstance;
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
        return m_player->streamSizeInFrames();
    else
        return m_player->streamSize();
}

/*
Return current stream position.
*/
size_t Player::streamPos(SizeType type) const
{
    if (type == FRAMES)
        return m_player->streamPosInFrames();
    else
        return m_player->streamPos();
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
    std::vector<std::string> stlList = m_player->supportedFormats();
    QList<QString> qtList(stlList.size());
    for (int i = 0; i < qtList.size(); i++)
        qtList[i] = QString::fromStdString(stlList.at(i));
    return qtList;
}

/*
Open an audio file.
*/
void Player::open(const QString& filePath)
{
    m_player->open(filePath.toStdString(), true);
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
This methods are wrapped between the event system of the simple-audio-libray and Qt.
*/
void Player::salStartNewFile(const std::string& filePath)
{
    emit startNewFile(QString::fromStdString(filePath));
}

void Player::salEndFile(const std::string& filePath)
{
    emit endFile(QString::fromStdString(filePath));
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
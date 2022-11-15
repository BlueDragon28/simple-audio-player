#include "windows/SMTCInterface.h"

SMTCInterface::SMTCInterface(QObject* parent) :
    QObject(parent)
{
    /*
    Set the callbacks for the System Media Transport Control interface.
    Every time the callbacks are called, the call is forwarded to the Qt Signal system.
    */
    m_smtc.setPlaybackCallback(SMTC::PlaybackType::PLAY_PAUSE,
        [this]() {
            emit this->playPause();
        });

    m_smtc.setPlaybackCallback(SMTC::PlaybackType::PREVIOUS,
        [this]() {
            emit this->previous();
        });

    m_smtc.setPlaybackCallback(SMTC::PlaybackType::NEXT,
        [this]() {
            emit this->next();
        });
}

SMTCInterface::~SMTCInterface()
{}

void SMTCInterface::setPlaybackStatus(SMTC::PlaybackStatus status)
{
    m_smtc.playbackStatusChanged(status);
}

void SMTCInterface::setTrackInfo(const QString &title, const QString &artists)
{
    m_smtc.trackChanged(
        title.toUtf8().constData(),
        artists.toUtf8().constData());
}

void SMTCInterface::setCanNext(bool value)
{
    m_smtc.setCanNext(value);
}

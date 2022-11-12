#include "windows/SMTCInterface.h"

SMTCInterface::SMTCInterface(QObject* parent) :
    QObject(parent)
{}

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

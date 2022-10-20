#include "dbus/SAPMPrisPlayer.h"
#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbusconnection.h>
#include <qdbusextratypes.h>

SAPMPrisPlayer::SAPMPrisPlayer(QObject* parent) :
    QObject(parent),
    m_playbackStatus("Stopped"),
    m_metadata(MetaData().toVariantMap()),
    m_position(0)
{
    QDBusConnection::sessionBus().registerObject(
        "/org/mpris/MediaPlayer2", 
        this, 
        QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllProperties);
}

void SAPMPrisPlayer::Next()
{
    qDebug() << u8"SAPMPrisPlayer::Next()";
}

void SAPMPrisPlayer::Previous()
{
    qDebug() << u8"SAPMPrisPlayer::Previous()";
}

void SAPMPrisPlayer::Pause()
{
    qDebug() << u8"SAPMPrisPlayer::Pause()";
}

void SAPMPrisPlayer::PlayPause()
{
    qDebug() << u8"SAPMPrisPlayer::PlayPause()";
}

void SAPMPrisPlayer::Stop()
{
    qDebug() << u8"SAPMPrisPlayer::Stop()";
}

void SAPMPrisPlayer::Play()
{
    qDebug() << u8"SAPMPrisPlayer::Play()";
}

void SAPMPrisPlayer::Seek(long long offset)
{
    qDebug() << u8"SAPMPrisPlayer::Seek(long long offset = " + QString::number(offset) + u8")";
}

void SAPMPrisPlayer::SetPosition(const QDBusObjectPath& id, long long position)
{
    qDebug() << u8"SAPMPrisPlayer::SetPosition(const QDBusObjectPath &id, long long position = " + QString::number(position) + u8")";
}

void SAPMPrisPlayer::OpenUri(const QString& uri)
{
    qDebug() << u8"SAPMPrisPlayer::OpenUri(const QString& uri = " + uri + u8")";
}

QString SAPMPrisPlayer::playbackStatus() const
{
    qDebug() << u8"SAPMPrisPlayer::playbackStatus() -> \"Stopped\"";
    return m_playbackStatus;
}

void SAPMPrisPlayer::setPlaybackStatus(PlaybackStatus status)
{
    // Updating the playback status.
    m_playbackStatus = playbackStatusEnumToString(status);

    emit playbackStatusChanged();
}

QString SAPMPrisPlayer::playbackStatusEnumToString(PlaybackStatus status)
{
    // Converting the enum class PlaybackStatus to string for dbus.
    if (status == PlaybackStatus::PLAYING)
        return "Playing";
    else if (status == PlaybackStatus::PAUSED)
        return "Paused";
    else
        return "Stopped";
}

double SAPMPrisPlayer::rate() const
{
    qDebug() << u8"SAPMPrisPlayer::rate()";
    return 1.0;
}

void SAPMPrisPlayer::setRate(double rate)
{
    qDebug() << u8"SAPMPrisPlayer::setRate(double rate = " + QString::number(rate) + u8")";
    emit rateChanged(); // Do not update anything.
}

QVariantMap SAPMPrisPlayer::metadata() const
{
    qDebug() << u8"SAPMPrisPlayer::metadata()";
    return QVariantMap();
}

void SAPMPrisPlayer::setMetadata(const MetaData &data)
{
    m_metadata = data.toVariantMap(); // Convert the struct to dbus valid value.
    emit metadataChanged();
}

QVariantMap SAPMPrisPlayer::MetaData::toVariantMap() const
{
    // Converting MetaData struct to valid dbus value.
    QVariantMap map;
    map["mpris:trackid"] = QDBusObjectPath(trackID);
    return map;
}

double SAPMPrisPlayer::volume() const
{
    qDebug() << u8"SAPMPrisPlayer::volume()";
    return 1.0;
}

void SAPMPrisPlayer::setVolume(double volume)
{
    qDebug() << u8"SAPMPrisPlayer::setVolume(double volume = " + QString::number(volume) + u8")";
    emit volumeChanged(); // Do not change, method not supported.
}

long long SAPMPrisPlayer::position() const
{
    qDebug() << u8"SAPMPrisPlayer::position()";
    return m_position; // Returning position in microsecond.
}

void SAPMPrisPlayer::setPosition(long long position)
{
    m_position = position * 1000; // Convert millisecond value to microsecond.
    emit positionChanged();
}

double SAPMPrisPlayer::minimumRate() const
{
    qDebug() << u8"SAPMPrisPlayer::minimumRate()";
    return 1.0;
}

double SAPMPrisPlayer::maximumRate() const
{
    qDebug() << u8"SAPMPrisPlayer::maximumRate()";
    return 1.0;
}

bool SAPMPrisPlayer::canNext() const
{
    qDebug() << u8"SAPMPrisPlayer::canNext()";
    return false;
}

bool SAPMPrisPlayer::canPrevious() const
{
    qDebug() << u8"SAPMPrisPlayer::canPrevious()";
    return false;
}

bool SAPMPrisPlayer::canPlay() const
{
    qDebug() << u8"SAPMPrisPlayer::canPlay()";
    return false;
}

bool SAPMPrisPlayer::canPause() const
{
    qDebug() << u8"SAPMPrisPlayer::canPause()";
    return false;
}

bool SAPMPrisPlayer::canSeek() const
{
    qDebug() << u8"SAPMPrisPlayer::canSeek()";
    return false;
}

bool SAPMPrisPlayer::canControl() const
{
    qDebug() << u8"SAPMPrisPlayer::canControl()";
    return false;
}
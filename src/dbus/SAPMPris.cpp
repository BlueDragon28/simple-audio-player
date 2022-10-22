#include "dbus/SAPMPris.h"
#include <QDebug>
#include <QtDBus/qdbusconnection.h>
#include <qdbusextratypes.h>
#include <qdbusmessage.h>

const char* SAPMPris::_serviceName = "org.mpris.MediaPlayer2.simpleaudioplayer";
const char* SAPMPris::_mprisObjectPath = "/org/mpris/MediaPlayer2";
const char* SAPMPris::_freeDesktopPath = "org.freedesktop.DBus.Properties";
const char* SAPMPris::_propertyChangedStr = "PropertiesChanged";
const char* SAPMPris::_mprisPlayerEntity = "org.mpris.MediaPlayer2.Player";

SAPMPris::SAPMPris(QObject* parent) :
    QObject(parent)
{
    QDBusConnection::sessionBus().registerService(_serviceName);
    QDBusConnection::sessionBus().registerObject(
        _mprisObjectPath, 
        this,
        QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllProperties);
}

SAPMPris::~SAPMPris()
{}

/*
Root MRPIS
*/
void SAPMPris::Raise()
{
    qDebug() << "SAPMPris::Raise()";
}

void SAPMPris::Quit()
{
    qDebug() << "SAPMPris::Quit()";
}

bool SAPMPris::canQuit() const
{
    qDebug() << "SAPMPris::Quit()";
    return false;
}

bool SAPMPris::canRaise() const
{
    qDebug() << "SAPMPris::Raise()";
    return false;
}

bool SAPMPris::hasTrackList() const
{
    qDebug() << "SAPMPRIS::HasTrackList()";
    return false;
}

QString SAPMPris::identity() const
{
    qDebug() << "SAPMPRIS::Identity()";
    return "simple-audio-player";
}

QStringList SAPMPris::supportedUriSchemes() const
{
    qDebug() << "SAPMPRIS::SupportedUriSchemes()";
    return { "file" };
}

QStringList SAPMPris::supportedMimeTypes() const
{
    qDebug() << "SAPMPRIS::SupportedMimeTypes()";
    return {};
}

/*
Player MPRIS
*/
void SAPMPris::Next()
{
    qDebug() << "SAPMPris::Next()";
}

void SAPMPris::Previous()
{
    qDebug() << "SAPMPris::Previous()";
}

void SAPMPris::Pause()
{
    qDebug() << "SAPMPris::Pause()";
}

void SAPMPris::PlayPause()
{
    qDebug() << "SAPMPris::PlayPause()";
}

void SAPMPris::Stop()
{
    qDebug() << "SAPMPris::Stop()";
}

void SAPMPris::Play()
{
    qDebug() << "SAPMPris::Play()";
}

void SAPMPris::Seek(long long offset)
{
    qDebug() << "SAPMPris::Seek()";
}

void SAPMPris::SetPosition(const QDBusObjectPath &id, long long position)
{
    qDebug() << "SAPMPris::SetPosition(id, position)";
}

void SAPMPris::OpenUri(const QString& uri)
{
    qDebug() << "SAPMPris::OpenUri()";
}

QString SAPMPris::playbackStatus() const
{
    qDebug() << "SAPMPris::playbackStatus()";

    return m_playbackStatus;
}

void SAPMPris::setPlaybackStatus(PlaybackStatus status)
{
    // Updating the playback status.
    m_playbackStatus = playbackStatusEnumToString(status);

    // Notify MPRIS of the change.
    notify(NotifyType::PLAYBACK_STATUS);
}

double SAPMPris::rate() const
{
    qDebug() << "SAPMPris::rate()";
    return 1.0;
}

void SAPMPris::setRate(double rate)
{
    qDebug() << "SAPMPris::setRate()";
}

QVariantMap SAPMPris::metadata() const
{
    qDebug() << "SAPMPris::metadata()";
    return m_metadata;
}

void SAPMPris::setMetadata(const MetaData& data)
{
    qDebug() << "SAPMPris::setMetadata()";
    m_metadata = data.toVariantMap();

    // Notify MPRIS of the change.
    notify(NotifyType::METADATA);
}

double SAPMPris::volume() const
{
    qDebug() << "SAPMPris::volume()";
    return 1.0;
}

void SAPMPris::setVolume(double volume)
{
    qDebug() << "SAPMPris::setVolume()";
}

long long SAPMPris::position() const
{
    qDebug() << "SAPMPris::position()";
    return m_position;
}

void SAPMPris::setPosition(long long position)
{
    m_position = position * 1000; // Convert millisecond value to microsecond.
}

double SAPMPris::minimumRate() const
{
    qDebug() << "SAPMPris::minimumRate()";
    return 1.0;
}

double SAPMPris::maximumRate() const
{
    qDebug() << "SAPMPris::maximumRate()";
    return 1.0;
}

bool SAPMPris::canNext() const
{
    qDebug() << "SAPMPris::canNext()";
    return false;
}

bool SAPMPris::canPrevious() const
{
    qDebug() << "SAPMPris::canPrevious()";
    return false;
}

bool SAPMPris::canPlay() const
{
    qDebug() << "SAPMPris::canPlay()";
    return true;
}

bool SAPMPris::canPause() const
{
    qDebug() << "SAPMPris::canPause()";
    return true;
}

bool SAPMPris::canSeek() const
{
    qDebug() << "SAPMPris::canSeek()";
    return false;
}

bool SAPMPris::canControl() const
{
    qDebug() << "SAPMPris::canControl()";
    return true;
}

/*
Helper methods
*/
QString SAPMPris::playbackStatusEnumToString(PlaybackStatus status)
{
    // Converting the enum class PlaybackStatus to string for dbus.
    if (status == PlaybackStatus::PLAYING)
        return "Playing";
    else if (status == PlaybackStatus::PAUSED)
        return "Paused";
    else
        return "Stopped";
}

QVariantMap SAPMPris::MetaData::toVariantMap() const
{
    // Convert MetaData to valid dbus values.
    QVariantMap map;
    map["mpris:trackid"] = QDBusObjectPath(QString("/simpleaudioplayer/%1").arg(trackID));
    map["xesam:title"] = "Hello There";
    map["xesam:album"] = "My Super Album";
    return map;
}

QString SAPMPris::notifyTypeToString(NotifyType type)
{
    // Converting the NotifyType to a string value for MPRIS.
    if (type == NotifyType::PLAYBACK_STATUS)
    {
        return "PlaybackStatus";
    }
    else if (type == NotifyType::METADATA)
    {
        return "Metadata";
    }
    else 
    {
        return "";
    }
}

QVariant SAPMPris::getNotifyValue(NotifyType type)
{
    if (type == NotifyType::PLAYBACK_STATUS)
    {
        return m_playbackStatus;
    }
    else if (type == NotifyType::METADATA)
    {
        return m_metadata;
    }
    else 
    {
        return QVariant();
    }
}

void SAPMPris::notify(NotifyType type, const QVariant& value)
{
    // Checking if the notify type is valid.
    if (type != NotifyType::UNKNOWN && value.isValid())
    {
        QString propertyName = notifyTypeToString(type);

        // Creating a DBus signal.
        QDBusMessage signal = QDBusMessage::createSignal(
            _mprisObjectPath, _freeDesktopPath, _propertyChangedStr);
        
        // Storing the property name and the new value into a variant map.
        QVariantMap map;
        map[propertyName] = value;

        // Creating the message args with the MPRIS entity (player), the property change and an empty string list.
        QVariantList args;
        args.append(_mprisPlayerEntity);
        args.append(map);
        args.append(QStringList());

        // Set the args into the signal.
        signal.setArguments(args);

        // Send the signal.
        QDBusConnection::sessionBus().send(signal);
    }
}

void SAPMPris::notify(NotifyType type)
{
    /*
    Getting the value based on the notify type 
    and send it to the other notify method.
    */
    const QVariant value = getNotifyValue(type);
    notify(type, value);
}

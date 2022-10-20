#include "SystemMediaControl.h"
#include "dbus/SAPMPris.h"
#include "dbus/SAPMPrisPlayer.h"
#include <memory>

#define TRACKID_INDEX "index"
#define TRACKID_PATH "path"

std::unique_ptr<SystemMediaControl> SystemMediaControl::_instance;
std::unique_ptr<SAPMPris> SystemMediaControl::dbusRoot;
std::unique_ptr<SAPMPrisPlayer> SystemMediaControl::dbusRootPlayer;

SystemMediaControl::SystemMediaControl()
{
    instance(); // Initializing the static variables.
}

SystemMediaControl* SystemMediaControl::instance()
{
    // Creating a singlestone instance and returning it.
    if (!_instance)
    {
        _instance = std::unique_ptr<SystemMediaControl>(new SystemMediaControl());
    }

    return _instance.get();
}

void SystemMediaControl::play()
{
    dbusRootPlayer->setPlaybackStatus(SAPMPrisPlayer::PlaybackStatus::PLAYING);
}

void SystemMediaControl::pause()
{
    dbusRootPlayer->setPlaybackStatus(SAPMPrisPlayer::PlaybackStatus::PAUSED);
}

void SystemMediaControl::stop()
{
    dbusRootPlayer->setPlaybackStatus(SAPMPrisPlayer::PlaybackStatus::STOPPED);
}

void SystemMediaControl::newTrack(const QVariantMap& vID)
{
    SAPMPrisPlayer::MetaData data;
    TrackID id = parseTrackID(vID);
    data.trackID = id.path.trimmed() + QString::number(id.index);
    dbusRootPlayer->setMetadata(data);
}

SystemMediaControl::TrackID SystemMediaControl::parseTrackID(const QVariantMap& vID)
{
    // Convert the javascript object into a C++ struct.
    TrackID id = {};

    if (vID.contains(TRACKID_INDEX) && vID[TRACKID_INDEX].canConvert<int>())
    {
        id.index = vID[TRACKID_INDEX].toInt();
    }

    if (vID.contains(TRACKID_PATH) && vID[TRACKID_INDEX].canConvert<QString>())
    {
        id.path = vID[TRACKID_PATH].toString();
    }

    return id;
}
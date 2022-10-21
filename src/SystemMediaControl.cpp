#include "SystemMediaControl.h"
#include "dbus/SAPMPris.h"
#include <memory>

#define TRACKID_INDEX "index"
#define TRACKID_PATH "path"

std::unique_ptr<SAPMPris> SystemMediaControl::dbusRoot;

SystemMediaControl::SystemMediaControl()
{}

void SystemMediaControl::init()
{
    // Initializing the static variables inside the class SystemMediaControl
    if (!dbusRoot)
    {
        dbusRoot = std::unique_ptr<SAPMPris>(new SAPMPris());
    }
}

void SystemMediaControl::play()
{
    dbusRoot->setPlaybackStatus(SAPMPris::PlaybackStatus::PLAYING);
    qDebug() << "playing";
}

void SystemMediaControl::pause()
{
    dbusRoot->setPlaybackStatus(SAPMPris::PlaybackStatus::PAUSED);
    qDebug() << "pausing";
}

void SystemMediaControl::stop()
{
    dbusRoot->setPlaybackStatus(SAPMPris::PlaybackStatus::STOPPED);
    qDebug() << "stopping";
}

void SystemMediaControl::newTrack(const QVariantMap& vID)
{
    SAPMPris::MetaData data;
    TrackID id = parseTrackID(vID);
    data.trackID = QString::number(id.index);
    dbusRoot->setMetadata(data);
    qDebug() << "newTrack: " + id.path;
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
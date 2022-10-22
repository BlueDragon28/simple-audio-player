#include "SystemMediaControl.h"
#include "dbus/SAPMPris.h"
#include <memory>

#define TRACKID_INDEX "index"
#define TRACKID_PATH "path"
#define TRACKID_TITLE "title"
#define TRACKID_ALBUM "album"
#define TRACKID_ARTISTS "artists"

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
    data.title = id.title;
    data.album = id.album;
    data.artists = id.artists;
    dbusRoot->setMetadata(data);
    qDebug() << "newTrack: " + id.path;
}

SystemMediaControl::TrackID SystemMediaControl::parseTrackID(const QVariantMap& vID)
{
    // Convert the javascript object into a C++ struct.
    TrackID id = {};

    if (isVariantMapContain<int>(vID, TRACKID_INDEX))
    {
        id.index = vID[TRACKID_INDEX].toInt();
    }

    if (isVariantMapContain<QString>(vID, TRACKID_PATH))
    {
        id.path = vID[TRACKID_PATH].toString();
    }

    if (isVariantMapContain<QString>(vID, TRACKID_TITLE))
    {
        id.title = vID[TRACKID_TITLE].toString();
    }

    if (isVariantMapContain<QString>(vID, TRACKID_ALBUM))
    {
        id.album = vID[TRACKID_ALBUM].toString();
    }

    if (isVariantMapContain<QString>(vID, TRACKID_ARTISTS))
    {
        id.artists = vID[TRACKID_ARTISTS].toString();
    }

    return id;
}
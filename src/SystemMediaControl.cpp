#include "SystemMediaControl.h"
#ifdef __linux__
#include "dbus/SAPMPris.h"
#elif WIN32
#include "windows/ListenMediaKeys.h"
#endif
#include <memory>

#define TRACKID_INDEX "index"
#define TRACKID_PATH "path"
#define TRACKID_TITLE "title"
#define TRACKID_ALBUM "album"
#define TRACKID_ARTISTS "artists"

#ifdef __linux__
std::unique_ptr<SAPMPris> SystemMediaControl::dbusMPRIS;
#endif

SystemMediaControl::SystemMediaControl()
#ifdef WIN32
    : m_windowsHook(nullptr)
#endif
{
#ifdef __linux__
    // Connect the signals of SAPMPris to the SystemMediaControl signals interface.
    connect(dbusMPRIS.get(), &SAPMPris::raise, this, &SystemMediaControl::raise);
    connect(dbusMPRIS.get(), &SAPMPris::playPause, this, &SystemMediaControl::playPause);
    connect(dbusMPRIS.get(), &SAPMPris::previous, this, &SystemMediaControl::previous);
    connect(dbusMPRIS.get(), &SAPMPris::next, this, &SystemMediaControl::next);
#elif WIN32
    ListenMediaKeys::init();
    m_windowsHook = ListenMediaKeys::instance();

    connect(m_windowsHook, &ListenMediaKeys::playPause, this, &SystemMediaControl::playPause);
    connect(m_windowsHook, &ListenMediaKeys::previous, this, &SystemMediaControl::previous);
    connect(m_windowsHook, &ListenMediaKeys::next, this, &SystemMediaControl::next);
#endif
}

void SystemMediaControl::init()
{
#ifdef __linux__
    // Initializing the static variables inside the class SystemMediaControl
    if (!dbusMPRIS)
    {
        dbusMPRIS = std::unique_ptr<SAPMPris>(new SAPMPris());
    }
#endif
}

void SystemMediaControl::play()
{
#ifdef __linux__
    dbusMPRIS->setPlaybackStatus(SAPMPris::PlaybackStatus::PLAYING);
#endif
}

void SystemMediaControl::pause()
{
#ifdef __linux__
    dbusMPRIS->setPlaybackStatus(SAPMPris::PlaybackStatus::PAUSED);
#endif
}

void SystemMediaControl::stop()
{
#ifdef __linux__
    dbusMPRIS->setPlaybackStatus(SAPMPris::PlaybackStatus::STOPPED);
#endif
}

void SystemMediaControl::newTrack(const QVariantMap& vID)
{
    TrackID id = parseTrackID(vID);

#ifdef __linux__
    SAPMPris::MetaData data;
    data.trackID = QString::number(id.index);
    data.title = id.title;
    data.album = id.album;
    data.artists = id.artists;
    dbusMPRIS->setMetadata(data);
#endif
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

void SystemMediaControl::canNext(bool value)
{
#ifdef __linux__
    // Notify MPRIS that there is or not another song after the current ont.
    dbusMPRIS->setCanNext(value);
#endif
}

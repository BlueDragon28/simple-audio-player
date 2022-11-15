#include "SystemMediaControl.h"
#ifdef __linux__
#include "dbus/SAPMPris.h"
#elif WIN32
#ifdef USE_SMTC
#include "windows/SMTCInterface.h"
#else
#include "windows/ListenMediaKeys.h"
#endif
#endif
#include <memory>

#define TRACKID_INDEX "index"
#define TRACKID_PATH "path"
#define TRACKID_TITLE "title"
#define TRACKID_ALBUM "album"
#define TRACKID_ARTISTS "artists"

#ifdef __linux__
std::unique_ptr<SAPMPris> SystemMediaControl::dbusMPRIS;
#elif WIN32
#ifdef USE_SMTC
std::unique_ptr<SMTCInterface> SystemMediaControl::smtcInterface;
#endif
#endif

SystemMediaControl::SystemMediaControl()
#ifdef WIN32
#ifndef USE_SMTC
    : m_windowsHook(nullptr)
#endif
#endif
{
#ifdef __linux__
    // Connect the signals of SAPMPris to the SystemMediaControl signals interface.
    connect(dbusMPRIS.get(), &SAPMPris::raise, this, &SystemMediaControl::raise);
    connect(dbusMPRIS.get(), &SAPMPris::playPause, this, &SystemMediaControl::playPause);
    connect(dbusMPRIS.get(), &SAPMPris::previous, this, &SystemMediaControl::previous);
    connect(dbusMPRIS.get(), &SAPMPris::next, this, &SystemMediaControl::next);
#elif WIN32
#ifdef USE_SMTC
    /*
    Passthrough the signals of the SMTCInterface to this class.
    */
    connect(smtcInterface.get(), &SMTCInterface::playPause, this, &SystemMediaControl::playPause);
    connect(smtcInterface.get(), &SMTCInterface::previous, this, &SystemMediaControl::previous);
    connect(smtcInterface.get(), &SMTCInterface::next, this, &SystemMediaControl::next);
#else
    ListenMediaKeys::init();
    m_windowsHook = ListenMediaKeys::instance();

    connect(m_windowsHook, &ListenMediaKeys::playPause, this, &SystemMediaControl::playPause);
    connect(m_windowsHook, &ListenMediaKeys::previous, this, &SystemMediaControl::previous);
    connect(m_windowsHook, &ListenMediaKeys::next, this, &SystemMediaControl::next);
#endif
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
#elif WIN32
#ifdef USE_SMTC
    if (!smtcInterface)
    {
        smtcInterface = std::unique_ptr<SMTCInterface>(new SMTCInterface());
    }
#endif
#endif
}

void SystemMediaControl::play()
{
#ifdef __linux__
    dbusMPRIS->setPlaybackStatus(SAPMPris::PlaybackStatus::PLAYING);
#elif WIN32
#ifdef USE_SMTC
    smtcInterface->setPlaybackStatus(SMTC::PlaybackStatus::PLAYING);
#endif
#endif
}

void SystemMediaControl::pause()
{
#ifdef __linux__
    dbusMPRIS->setPlaybackStatus(SAPMPris::PlaybackStatus::PAUSED);
#elif WIN32
#ifdef USE_SMTC
    smtcInterface->setPlaybackStatus(SMTC::PlaybackStatus::PAUSED);
#endif
#endif
}

void SystemMediaControl::stop()
{
#ifdef __linux__
    dbusMPRIS->setPlaybackStatus(SAPMPris::PlaybackStatus::STOPPED);
#elif WIN32
#ifdef USE_SMTC
    smtcInterface->setPlaybackStatus(SMTC::PlaybackStatus::STOPPED);
#endif
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
#elif WIN32
#ifdef USE_SMTC
    smtcInterface->setTrackInfo(id.title, id.artists);
#endif
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
    // Notify MPRIS that there is or not another song after the current one.
    dbusMPRIS->setCanNext(value);
#elif WIN32
#ifdef USE_SMTC
    smtcInterface->setCanNext(value);
#endif
#endif
}

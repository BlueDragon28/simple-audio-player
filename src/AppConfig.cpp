#include "AppConfig.h"
#include "Player.h"
#include <simple-audio-library/Common.h>
#include <QSettings>
#include <algorithm>
#include <qfileinfo.h>
#include <qsettings.h>
#include <qvariant.h>

#define WINDOW_CONFIG_NAME "mainWindow"
#define WINDOW_MAKE_CONFIG_NAME(name) QString(WINDOW_CONFIG_NAME) + '/' + name // helper function to make config window settings name
#define WINDOW_X "x"
#define WINDOW_Y "y"
#define WINDOW_WIDTH "width"
#define WINDOW_HEIGHT "height"
#define WINDOW_MAXIMIZED "maximized"
#define WINDOW_DEFAULT_POS 256
#define WINDOW_DEFAULT_SIZE_WIDTH 1024
#define WINDOW_DEFAULT_SIZE_HEIGHT 512

#define MUSIC_COLLECTION_CONFIG_NAME "collection"
#define MUSIC_COLLECTION_MAKE_CONFIG_NAME(name) QString(MUSIC_COLLECTION_CONFIG_NAME) + '/' + name
#define MUSIC_COLLECTION_PATH_LIST "dirList"

#define PLAYLIST_CONFIG_NAME "playlist"
#define PLAYLIST_MAKE_CONFIG_NAME(name) QString(PLAYLIST_CONFIG_NAME) + '/' + name
#define PLAYLIST_LAST_OPENED_PLAYLIST_PATH "jsonPath"

#define STREAMOPTION_CONFIG_NAME "stream"
#define STREAMOPTION_MAKE_CONFIG_NAME(name) QString(STREAMOPTION_CONFIG_NAME) + '/' + name
#define STREAMOPTION_BACKEND_AUDIO "backend"

AppConfig::WindowSettings AppConfig::mainWindowSettings = {
    WINDOW_DEFAULT_POS, // X
    WINDOW_DEFAULT_POS, // Y
    WINDOW_DEFAULT_SIZE_WIDTH, // width
    WINDOW_DEFAULT_SIZE_HEIGHT, // height
    false // miximize
};
bool AppConfig::isMainWindowSettings = true;

AppConfig::MusicCollectionList AppConfig::m_musicCollectionPathList = {
    QStringList(),
    false
};

QString AppConfig::m_lastOpenedPlaylist = QString();

AppConfig::BackendAudioSetting AppConfig::m_backendAudioOption = {
    SAL::BackendAudio::SYSTEM_DEFAULT,
    false
};

QSettings AppConfig::openSettings()
{
#ifdef NDEBUG
    return QSettings("BlueDragon28", "simple-audio-player");
#else
    return QSettings("BlueDragon28", "simple-audio-player_debug");
#endif
}

void AppConfig::loadConfig()
{
    // Preparing to read settings.
    QSettings settings = openSettings();

    loadWindowStatus(settings);
    loadMusicCollectionPathList(settings);
    loadLastOpenedPlaylistPath(settings);
    loadStreamOptions(settings);
}

void AppConfig::saveConfig()
{
    // Preparing to write settings.
    QSettings settings = openSettings();

    saveWindowStatus(settings);
    saveMusicCollectionPathList(settings);
    saveLastOpenedPlaylistPath(settings);
    saveStreamOptions(settings);
}

QMap<QString, QVariant> AppConfig::getMainWindowSettings()
{
    /*
    Building a javascript object width the window data.
    */
    QMap<QString, QVariant> settings;

    if (isMainWindowSettings)
    {
        settings[WINDOW_X] = std::max(0, mainWindowSettings.x);
        settings[WINDOW_Y] = std::max(0, mainWindowSettings.y);
        settings[WINDOW_WIDTH] = std::max(0, mainWindowSettings.width);
        settings[WINDOW_HEIGHT] = std::max(0, mainWindowSettings.height);
        settings[WINDOW_MAXIMIZED] = mainWindowSettings.maximize;
    }

    return settings;
}

void AppConfig::setMainWindowSettings(const QMap<QString, QVariant> &s)
{
    // Retrieve the data from the javascript object.
    // Retrieve position X and Y.
    if (s.contains(WINDOW_X))
    {
        if (s[WINDOW_X].canConvert<int>())
        {
            mainWindowSettings.x = std::max(0, qvariant_cast<int>(s[WINDOW_X]));
        }
        else 
        {
            mainWindowSettings.x = WINDOW_DEFAULT_POS;
        }
    }

    if (s.contains(WINDOW_Y))
    {
        if (s[WINDOW_Y].canConvert<int>())
        {
            mainWindowSettings.y = std::max(0, qvariant_cast<int>(s[WINDOW_Y]));
        }
        else 
        {
            mainWindowSettings.y = WINDOW_DEFAULT_POS;
        } 
    }

    // Retrive size.
    if (s.contains(WINDOW_WIDTH))
    {
        if (s[WINDOW_WIDTH].canConvert<int>())
        {
            mainWindowSettings.width = std::max(0, qvariant_cast<int>(s[WINDOW_WIDTH]));
        }
        else 
        {
            mainWindowSettings.width = WINDOW_DEFAULT_SIZE_WIDTH;
        }
    }

    if (s.contains(WINDOW_HEIGHT))
    {
        if (s[WINDOW_HEIGHT].canConvert<int>())
        {
            mainWindowSettings.height = std::max(0, qvariant_cast<int>(s[WINDOW_HEIGHT]));
        }
        else
        {
            mainWindowSettings.height = WINDOW_DEFAULT_SIZE_HEIGHT;
        }
    }

    // Retrieve maximize settings.
    if (s.contains(WINDOW_MAXIMIZED))
    {
        if (s[WINDOW_MAXIMIZED].canConvert<bool>())
        {
            mainWindowSettings.maximize = qvariant_cast<bool>(s[WINDOW_MAXIMIZED]);
        }
        else 
        {
            mainWindowSettings.maximize = true;
        }
    }
}

QStringList AppConfig::getMusicCollectionPathList(bool* exists)
{
    // If exists is valid, set it to the value of m_musicCollectionPathList.exists.
    if (exists)
    {
        *exists = m_musicCollectionPathList.exists;
    }

    // Returning the list of m_musicCollectionPathList.
    return m_musicCollectionPathList.path;
}

void AppConfig::setMusicCollectionPathList(const QStringList& pathList)
{
    // Update the path list.
    m_musicCollectionPathList.path = pathList;
    m_musicCollectionPathList.exists = true;
}

QString AppConfig::getLastOpenedPlaylistPath()
{
    return m_lastOpenedPlaylist;
}

void AppConfig::setLastOpenedPlaylistPath(const QString& path)
{
    m_lastOpenedPlaylist = path.trimmed();
}

void AppConfig::setBackendAudioSetting(SAL::BackendAudio backend)
{
    m_backendAudioOption = {
        backend,
        true
    };

    Player::getInstance()->getPlayer()->setBackendAudio(backend);
}

SAL::BackendAudio AppConfig::getBackendAudioSetting()
{
    if (!m_backendAudioOption.exists) return SAL::BackendAudio::SYSTEM_DEFAULT;

    return m_backendAudioOption.backend;
}

void AppConfig::saveWindowStatus(QSettings& settings)
{
    if (settings.isWritable())
    {
        // Save the value into the settings registry/config directory.
        settings.setValue(WINDOW_MAKE_CONFIG_NAME(WINDOW_X), mainWindowSettings.x);
        settings.setValue(WINDOW_MAKE_CONFIG_NAME(WINDOW_Y), mainWindowSettings.y);
        settings.setValue(WINDOW_MAKE_CONFIG_NAME(WINDOW_WIDTH), mainWindowSettings.width);
        settings.setValue(WINDOW_MAKE_CONFIG_NAME(WINDOW_HEIGHT), mainWindowSettings.height);
        settings.setValue(WINDOW_MAKE_CONFIG_NAME(WINDOW_MAXIMIZED), mainWindowSettings.maximize);
    }
}

void AppConfig::loadWindowStatus(QSettings& settings)
{
    // Reading the window status from the registry/config directory using the settings object.
    QVariant vX = settings.value(WINDOW_MAKE_CONFIG_NAME(WINDOW_X));
    if (vX.isValid() && vX.canConvert<int>())
    {
        mainWindowSettings.x = vX.toInt();
    }

    QVariant vY = settings.value(WINDOW_MAKE_CONFIG_NAME(WINDOW_Y));
    if (vY.isValid() && vY.canConvert<int>())
    {
        mainWindowSettings.y = vY.toInt();
    }

    QVariant vWidth = settings.value(WINDOW_MAKE_CONFIG_NAME(WINDOW_WIDTH));
    if (vWidth.isValid() && vWidth.canConvert<int>())
    {
        mainWindowSettings.width = vWidth.toInt();
    }

    QVariant vHeight = settings.value(WINDOW_MAKE_CONFIG_NAME(WINDOW_HEIGHT));
    if (vHeight.isValid() && vHeight.canConvert<int>())
    {
        mainWindowSettings.height = vHeight.toInt();
    }

    QVariant vMaximized = settings.value(WINDOW_MAKE_CONFIG_NAME(WINDOW_MAXIMIZED));
    if (vMaximized.isValid() && vMaximized.canConvert<bool>())
    {
        mainWindowSettings.maximize = vMaximized.toBool();
    }
}

void AppConfig::saveMusicCollectionPathList(QSettings& settings)
{
    if (settings.isWritable())
    {
        // Save only the music collectin path list if the user set it.
        if (m_musicCollectionPathList.exists)
        {
            if (!m_musicCollectionPathList.path.isEmpty())
            {
                settings.setValue(MUSIC_COLLECTION_MAKE_CONFIG_NAME(MUSIC_COLLECTION_PATH_LIST), m_musicCollectionPathList.path);
            }
            else
            {
                settings.setValue(MUSIC_COLLECTION_MAKE_CONFIG_NAME(MUSIC_COLLECTION_PATH_LIST), "");
            }
        }
    }
}

void AppConfig::loadMusicCollectionPathList(QSettings& settings)
{
    // Load the music collection path list from the registry/config directory.
    QVariant vPathList = settings.value(MUSIC_COLLECTION_MAKE_CONFIG_NAME(MUSIC_COLLECTION_PATH_LIST));
    if (vPathList.isValid() && vPathList.canConvert<QStringList>())
    {
        m_musicCollectionPathList.path = vPathList.toStringList();
        m_musicCollectionPathList.exists = true;

        // If there is only one item in the list and its an empty string, then, clear the list.
        if (m_musicCollectionPathList.path.size() == 1)
        {
            if (m_musicCollectionPathList.path.at(0).isEmpty())
            {
                m_musicCollectionPathList.path.clear();
            }
        }
    }
}

void AppConfig::saveLastOpenedPlaylistPath(QSettings& settings)
{
    if(!settings.isWritable()) {
        return;
    }

    if (m_lastOpenedPlaylist.isEmpty())
    {
        settings.remove(PLAYLIST_MAKE_CONFIG_NAME(PLAYLIST_LAST_OPENED_PLAYLIST_PATH));
    }

    settings.setValue(PLAYLIST_MAKE_CONFIG_NAME(PLAYLIST_LAST_OPENED_PLAYLIST_PATH), m_lastOpenedPlaylist);
}

void AppConfig::loadLastOpenedPlaylistPath(QSettings& settings)
{
    QVariant vPlaylistPath = 
        settings.value(PLAYLIST_MAKE_CONFIG_NAME(PLAYLIST_LAST_OPENED_PLAYLIST_PATH));

    if (!vPlaylistPath.isValid() || !vPlaylistPath.canConvert<QString>() || !vPlaylistPath.convert(QMetaType::fromType<QString>()))
    {
        return;
    }

    const QString playlistJSONPath = vPlaylistPath.toString();

    if (!QFileInfo::exists(playlistJSONPath))
    {
        return;
    }

    m_lastOpenedPlaylist = playlistJSONPath;
}

void AppConfig::saveStreamOptions(QSettings& settings)
{
    if (!settings.isWritable()) return;

    /*
    =================================================
    ================ BACKEND AUDIO ==================
    =================================================
    */
    if (m_backendAudioOption.exists) {
        settings.setValue(
            STREAMOPTION_MAKE_CONFIG_NAME(STREAMOPTION_BACKEND_AUDIO),
            (int)m_backendAudioOption.backend
        );
    }
}

void AppConfig::loadStreamOptions(QSettings& settings)
{
    /*
    =================================================
    ================ BACKEND AUDIO ==================
    =================================================
    */
    QVariant vBackendAudio = settings.value(
            STREAMOPTION_MAKE_CONFIG_NAME(STREAMOPTION_BACKEND_AUDIO)
    );
    if (vBackendAudio.isValid() && 
        vBackendAudio.canConvert<int>() && 
        vBackendAudio.convert(QMetaType::fromType<int>()))
    {
        m_backendAudioOption = {
            (SAL::BackendAudio)qvariant_cast<int>(vBackendAudio),
            true
        };
    }
}

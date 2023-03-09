#ifndef SIMPLE_AUDIO_PLAYER_APPCONFIG_H_
#define SIMPLE_AUDIO_PLAYER_APPCONFIG_H_

#include "MusicCollectionList.h"
#include <QObject>
#include <QSettings>
#include <qmap.h>
#include <qqmlintegration.h>
#include <qsettings.h>
#include <qtmetamacros.h>

/*
Including all the configurations of the application.
*/
class AppConfig : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    /*
    Loading settings.
    */
    static void loadConfig();

    /*
    Saving settings.
    */
    static void saveConfig();

    /*
    Return the settings of the window has a javascript object.
    */
    Q_INVOKABLE static QMap<QString, QVariant> getMainWindowSettings();

    /*
    Update in memory settings based on a javascript object.
    */
    Q_INVOKABLE static void setMainWindowSettings(const QMap<QString, QVariant>& settings);

    /*
    Return the music collection path list.
    This is the list to listen to get the music collection (ex: albums)
    */
    static QStringList getMusicCollectionPathList(bool* exists = nullptr);

    /*
    Save in memory settings the music collection path list to listen.
    */
    static void setMusicCollectionPathList(const QStringList& pathList);

    /*
    Get the last opened playlist file path.
    */
    static QString getLastOpenedPlaylistPath();

    /*
    Set the last opened playlist file path.
    */
    static void setLastOpenedPlaylistPath(const QString& path);

private:
    static QSettings openSettings();

    /*
    Save window status settings into registry/config directory.
    */
    static void saveWindowStatus(QSettings& settings);

    /*
    Load window status settings from registry/config directory
    */
    static void loadWindowStatus(QSettings& settings);

    /*
    Save the music collection path list into registry/config directory.
    */
    static void saveMusicCollectionPathList(QSettings& settings);

    /*
    Load the music collection path list from registry/config directory.
    */
    static void loadMusicCollectionPathList(QSettings& settings);

    /*
    Save the last opened playlist path into registry/config directory.
    */
    static void saveLastOpenedPlaylistPath(QSettings& settings);

    /*
    Load the last opened playlist path from registry/config directory.
    */
    static void loadLastOpenedPlaylistPath(QSettings& settings);

    struct WindowSettings
    {
        int x;
        int y;
        int width;
        int height;
        bool maximize;
    };
    static WindowSettings mainWindowSettings;
    static bool isMainWindowSettings;

    struct MusicCollectionList
    {
        QStringList path;
        bool exists;
    };
    static MusicCollectionList m_musicCollectionPathList;

    static QString m_lastOpenedPlaylist;
};

#endif // SIMPLE_AUDIO_PLAYER_APPCONFIG_H_

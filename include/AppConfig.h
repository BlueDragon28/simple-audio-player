#ifndef SIMPLE_AUDIO_PLAYER_APPCONFIG_H_
#define SIMPLE_AUDIO_PLAYER_APPCONFIG_H_

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
};

#endif // SIMPLE_AUDIO_PLAYER_APPCONFIG_H_
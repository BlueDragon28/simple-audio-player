#ifndef SIMPLE_AUDIO_PLAYER_APPCONFIG_H_
#define SIMPLE_AUDIO_PLAYER_APPCONFIG_H_

#include <QObject>
#include <QSettings>
#include <qqmlintegration.h>
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

private:
    static QSettings openSettings();
};

#endif // SIMPLE_AUDIO_PLAYER_APPCONFIG_H_
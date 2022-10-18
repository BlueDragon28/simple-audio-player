#include "AppConfig.h"
#include <QSettings>
#include <algorithm>
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

AppConfig::WindowSettings AppConfig::mainWindowSettings = {
    WINDOW_DEFAULT_POS, // X
    WINDOW_DEFAULT_POS, // Y
    WINDOW_DEFAULT_SIZE_WIDTH, // width
    WINDOW_DEFAULT_SIZE_HEIGHT, // height
    false // miximize
};
bool AppConfig::isMainWindowSettings = true;

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
}

void AppConfig::saveConfig()
{
    // Preparing to write settings.
    QSettings settings = openSettings();

    saveWindowStatus(settings);
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
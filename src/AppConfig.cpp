#include "AppConfig.h"
#include <QSettings>
#include <algorithm>
#include <qvariant.h>

#define WINDOW_X "x"
#define WINDOW_Y "y"
#define WINDOW_WIDTH "width"
#define WINDOW_HEIGHT "height"
#define WINDOW_MAXIMIZE "miximize"
#define WINDOW_DEFAULT_SIZE_AND_POS 256

AppConfig::WindowSettings AppConfig::mainWindowSettings = {};
bool AppConfig::isMainWindowSettings = false;

QSettings AppConfig::openSettings()
{
#ifndef NDEBUG
    return QSettings("BlueDragon28", "simple-audio-library");
#else
    return QSettings("BlueDragon28", "simple-audio-library");
#endif
}

void AppConfig::loadConfig()
{
    // Preparing to read settings.
    QSettings settings = openSettings();
}

void AppConfig::saveConfig()
{
    // Preparing to write settings.
    QSettings settings = openSettings();
}

QMap<QString, QVariant> AppConfig::getMainWindowSettings()
{
    /*
    Building a javascript object width the window data.
    */
    QMap<QString, QVariant> settings;

    if (isMainWindowSettings)
    {
        settings[WINDOW_X] = mainWindowSettings.x;
        settings[WINDOW_Y] = mainWindowSettings.y;
        settings[WINDOW_WIDTH] = mainWindowSettings.width;
        settings[WINDOW_HEIGHT] = mainWindowSettings.height;
        settings[WINDOW_MAXIMIZE] = mainWindowSettings.maximize;
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
            mainWindowSettings.x = WINDOW_DEFAULT_SIZE_AND_POS;
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
            mainWindowSettings.y = WINDOW_DEFAULT_SIZE_AND_POS;
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
            mainWindowSettings.width = WINDOW_DEFAULT_SIZE_AND_POS;
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
            mainWindowSettings.height = WINDOW_DEFAULT_SIZE_AND_POS;
        }
    }

    // Retrieve maximize settings.
    if (s.contains(WINDOW_MAXIMIZE))
    {
        if (s[WINDOW_MAXIMIZE].canConvert<bool>())
        {
            mainWindowSettings.maximize = qvariant_cast<bool>(s[WINDOW_MAXIMIZE]);
        }
        else 
        {
            mainWindowSettings.maximize = true;
        }
    }
}
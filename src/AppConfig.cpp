#include "AppConfig.h"
#include <QSettings>

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
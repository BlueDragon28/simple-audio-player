#include "dbus/SAPMPris.h"
#include <QDebug>
#include <QtDBus/qdbusconnection.h>

SAPMPris::SAPMPris()
{
    QDBusConnection::sessionBus().registerService("org.mpris.MediaPlayer2.simpleaudioplayer");
    QDBusConnection::sessionBus().registerObject(
        "/org/mpris/MediaPlayer2", 
        this, 
        QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllProperties);
}

SAPMPris::~SAPMPris()
{}

void SAPMPris::Raise()
{
    qDebug() << "SAPMPris::Raise()";
}

void SAPMPris::Quit()
{
    qDebug() << "SAPMPris::Quit()";
}

bool SAPMPris::canQuit() const
{
    qDebug() << "SAPMPris::Quit()";
    return false;
}

bool SAPMPris::canRaise() const
{
    qDebug() << "SAPMPris::Raise()";
    return false;
}

bool SAPMPris::hasTrackList() const
{
    qDebug() << "SAPMPRIS::HasTrackList()";
    return false;
}

QString SAPMPris::identity() const
{
    qDebug() << "SAPMPRIS::Identity()";
    return "simple-audio-player";
}

QStringList SAPMPris::supportedUriSchemes() const
{
    qDebug() << "SAPMPRIS::SupportedUriSchemes()";
    return { "file" };
}

QStringList SAPMPris::supportedMimeTypes() const
{
    qDebug() << "SAPMPRIS::SupportedMimeTypes()";
    return {};
}
#ifndef SIMPLEAUDIOPLAYER_SYSTEM_MEDIA_CONTROL_H_
#define SIMPLEAUDIOPLAYER_SYSTEM_MEDIA_CONTROL_H_

#include "dbus/SAPMPris.h"
#include <QObject>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

/*
This class is an interface to communicate with the system about the state of the playback.
*/
class SystemMediaControl : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    SystemMediaControl();

    struct TrackID
    {
        int index = 0;
        QString path;
        QString title;
        QString album;
        QString artists;
    };

    /*
    Initialize static variables
    */
    static void init();

    Q_INVOKABLE static void play(); // Telling the system that a playback is playing.
    Q_INVOKABLE static void pause(); // Telling the system that a playback is paused.
    Q_INVOKABLE static void stop(); // Telling the system that the playback system stopped.
    Q_INVOKABLE static void newTrack(const QVariantMap& id); // Telling the system that a new track is playing.

private:
    static TrackID parseTrackID(const QVariantMap& vID);

    /*
    Helper function to help check if a variant is valid.
    */
    template<typename T>
    inline static bool isVariantMapContain(const QVariantMap& vm, const QString& key);

    static std::unique_ptr<SAPMPris> dbusRoot;
};

template<typename T>
inline bool SystemMediaControl::isVariantMapContain(const QVariantMap& vm, const QString& key)
{
    return (vm.contains(key) && vm[key].isValid() && vm[key].canConvert<T>());
}

#endif // SIMPLEAUDIOPLAYER_SYSTEM_MEDIA_CONTROL_H_
#ifndef SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_

#include "EventCallback.h"
#include "SpotifyAuthorizationPKCE.h"
#include <functional>
#include <qobject.h>
#include <qnetworkrequest.h>
#include <qtmetamacros.h>

class SpotifyPlayer : public QObject 
{
    Q_OBJECT

public:
    SpotifyPlayer(SpotifyAuthorizationPKCE* spotifyAccess, QObject* parent = nullptr);
    virtual ~SpotifyPlayer();

public slots:
    void play(const QVariantMap& playArguments);

signals:
    void isPlaying();

private:
    void handlePlayResponse(QNetworkReply* reply);

    void fetchAvailableDevices();
    void handleAvailableDeviceResponse(QNetworkReply* reply);

    EventCallback m_eventCallback;

    SpotifyAuthorizationPKCE* m_spotifyAccess;
    
    QString m_deviceID;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_

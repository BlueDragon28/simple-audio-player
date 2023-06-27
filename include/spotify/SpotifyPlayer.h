#ifndef SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_

#include "EventCallback.h"
#include "SpotifyAuthorizationPKCE.h"
#include <functional>
#include <qjsondocument.h>
#include <qnetworkreply.h>
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
    void resume();
    void pause();
    void toggleShuffle(bool shuffleState);

signals:
    void isPlaying();
    void isResuming();
    void isPausing();

private:
    void handlePlayResponse(QNetworkReply* reply);
    void handleResumeResponse(QNetworkReply* reply);
    void handlePauseResponse(QNetworkReply* reply);
    void handleToggleShuffleResponse(QNetworkReply* reply);

    void fetchAvailableDevices();
    void handleAvailableDeviceResponse(QNetworkReply* reply);

    static bool _isResponseAnError(const QByteArray& data);
    static bool _isError(const QJsonDocument& jsonDocument);
    static QJsonDocument _parseJSon(const QByteArray& data, bool* isError = nullptr);

    EventCallback m_eventCallback;

    SpotifyAuthorizationPKCE* m_spotifyAccess;
    
    QString m_deviceID;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_

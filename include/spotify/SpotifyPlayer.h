#ifndef SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_

#include "SpotifyAuthorizationPKCE.h"
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

    SpotifyAuthorizationPKCE* m_spotifyAccess;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYPLAYER_H_

#ifndef SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_

#include "spotify/SpotifyAuthorizationPKCE.h"
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class SpotifyAPI : public QObject 
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    SpotifyAPI();
    virtual ~SpotifyAPI();

signals:
    void error();

public slots:
    void authenticate();
    void setClientID(const QString& clientID);

private:
    SpotifyAuthorizationPKCE* m_spotifyAuth;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_


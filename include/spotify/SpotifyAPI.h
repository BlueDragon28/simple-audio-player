#ifndef SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_

#include "spotify/SpotifyAuthorizationPKCE.h"
#include "spotify/SpotifyUserInfo.h"
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class SpotifyAPI : public QObject 
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(SpotifyUserInfo* userInfo READ userInfo)

public:
    SpotifyAPI();
    virtual ~SpotifyAPI();

    SpotifyUserInfo* userInfo();

signals:
    void error();

public slots:
    void authenticate();
    void setClientID(const QString& clientID);

private:
    SpotifyAuthorizationPKCE* m_spotifyAuth;
    SpotifyUserInfo* m_userInfo;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_


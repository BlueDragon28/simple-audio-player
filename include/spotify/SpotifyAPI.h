#ifndef SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_

#include "spotify/SpotifyAuthorizationPKCE.h"
#include "spotify/SpotifyUserInfo.h"
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

/*
Default class to handle interaction with the Spotify API.
*/
class SpotifyAPI : public QObject 
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(SpotifyUserInfo* userInfo READ userInfo NOTIFY userInfoChanged)

public:
    SpotifyAPI();
    virtual ~SpotifyAPI();

    SpotifyUserInfo* userInfo();

signals:
    void error();
    void authenticated();

    void userInfoChanged(); // Disable warning

public slots:
    void authenticate();
    void setClientID(const QString& clientID);

private:
    // Get information about the user (username, country...)
    void updateProfile();
    void updateProfileHandler(QNetworkReply* reply);

    SpotifyAuthorizationPKCE* m_spotifyAuth;
    SpotifyUserInfo* m_userInfo;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_


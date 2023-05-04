#ifndef SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_

#include "SpotifyPlaylist.h"
#include "spotify/SpotifyAuthorizationPKCE.h"
#include "spotify/SpotifyTokenSaver.h"
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
    Q_PROPERTY(SpotifyPlaylist* userPlaylist READ userPlaylist NOTIFY userPlaylistChanged)

public:
    SpotifyAPI();
    virtual ~SpotifyAPI();

    SpotifyUserInfo* userInfo();
    SpotifyPlaylist* userPlaylist();

signals:
    void error();
    void authenticated();

    void userInfoChanged(); // Disable warning
    void userPlaylistChanged(); // Disable warning

public slots:
    void restoreCredential();
    void authenticate(bool saveToken);
    void setClientID(const QString& clientID);

private:
    // Get information about the user (username, country...)
    void updateProfile();
    void updateProfileHandler(QNetworkReply* reply);

    // Get user playlists informations
    void fetchUserPlaylists();

    // When authenticated, save the refresh token
    void saveRefreshToken();
    void tokenRestoredHandler(const QString& refreshToken);

    bool m_saveToken;
    bool m_firstTokenRefresh;

    SpotifyAuthorizationPKCE* m_spotifyAuth;
    SpotifyUserInfo* m_userInfo;
    SpotifyTokenSaver* m_tokenSaver;
    SpotifyPlaylist* m_userPlaylist;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_


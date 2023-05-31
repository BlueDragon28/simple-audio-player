#ifndef SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_

#include "SpotifyPlayer.h"
#include "SpotifyPlaylist.h"
#include "SpotifyReceivedPlaylistElement.h"
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
    Q_PROPERTY(SpotifyPlayer* player READ spotifyPlayer NOTIFY spotifyPlayerChanged)

public:
    SpotifyAPI();
    virtual ~SpotifyAPI();

    SpotifyUserInfo* userInfo();
    SpotifyPlaylist* userPlaylist();
    SpotifyPlayer* spotifyPlayer();

signals:
    void error();
    void authenticated();
    void loggedout();

    void userInfoChanged(); // Disable warning
    void userPlaylistChanged(); // Disable warning
    void spotifyPlayerChanged(); // Disable warning

    void receivedPlaylistDetails(SpotifyReceivedPlaylistElement* playlist);

public slots:
    void restoreCredential();
    void authenticate(bool saveToken);
    void setClientID(const QString& clientID);

    void displayPlaylistDetails(const QUrl& playlistHRef);

    void logout();

    bool isAuthenticated() const;

private:
    // Get information about the user (username, country...)
    void updateProfile();
    void updateProfileHandler(QNetworkReply* reply);

    // Get user playlists informations
    void fetchUserPlaylists();
    void fetchPrevNextPlaylistsPage(const QUrl& url);

    // When authenticated, save the refresh token
    void saveRefreshToken();
    void tokenRestoredHandler(const QString& refreshToken);

    // Get playlist details
    void handlePlaylistDetails(QNetworkReply* reply);

    bool m_saveToken;
    bool m_firstTokenRefresh;

    SpotifyAuthorizationPKCE* m_spotifyAuth;
    SpotifyUserInfo* m_userInfo;
    SpotifyTokenSaver* m_tokenSaver;
    SpotifyPlaylist* m_userPlaylist;
    SpotifyPlayer* m_spotifyPlayer;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYAPI_H_


#include "spotify/SpotifyAPI.h"
#include "spotify/SpotifyAuthorizationPKCE.h"
#include "spotify/SpotifyUserInfo.h"

SpotifyAPI::SpotifyAPI() : 
    QObject(),
    m_spotifyAuth(new SpotifyAuthorizationPKCE(this)),
    m_userInfo(new SpotifyUserInfo(this))
{
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::errorThrown, this, &SpotifyAPI::error);
}

SpotifyAPI::~SpotifyAPI() 
{}

void SpotifyAPI::authenticate()
{
    m_spotifyAuth->grant();
}

void SpotifyAPI::setClientID(const QString& clientID)
{
    m_spotifyAuth->setClientID(clientID);
}

SpotifyUserInfo* SpotifyAPI::userInfo()
{
    return m_userInfo;
}
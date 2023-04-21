#include "spotify/SpotifyAPI.h"
#include "spotify/SpotifyAuthorizationPKCE.h"

SpotifyAPI::SpotifyAPI() : 
    QObject(),
    m_spotifyAuth(new SpotifyAuthorizationPKCE(this))
{}

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
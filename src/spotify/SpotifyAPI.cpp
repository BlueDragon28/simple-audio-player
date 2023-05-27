#include "spotify/SpotifyAPI.h"
#include "SpotifyPlaylist.h"
#include "SpotifyReceivedPlaylistElement.h"
#include "spotify/SpotifyAuthorizationPKCE.h"
#include "spotify/SpotifyTokenSaver.h"
#include "spotify/SpotifyUserInfo.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnamespace.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>
#include <qnetworkreply.h>
#include <qurlquery.h>

SpotifyAPI::SpotifyAPI() : 
    QObject(),
    m_saveToken(false),
    m_firstTokenRefresh(true),
    m_spotifyAuth(new SpotifyAuthorizationPKCE(this)),
    m_userInfo(new SpotifyUserInfo(this)),
    m_tokenSaver(new SpotifyTokenSaver(this)),
    m_userPlaylist(new SpotifyPlaylist(this))
{
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::errorThrown, this, &SpotifyAPI::error);
    connect(m_userPlaylist, &SpotifyPlaylist::error, this, &SpotifyAPI::error);
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::authenticated, this, &SpotifyAPI::authenticated);
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::authenticated, [this](){m_firstTokenRefresh = false;});

    // When authenticated, request user information and get user playlist.
    connect(this, &SpotifyAPI::authenticated, this, &SpotifyAPI::updateProfile);
    connect(this, &SpotifyAPI::authenticated, this, &SpotifyAPI::fetchUserPlaylists);

    // When authenticated, save the refresh token
    connect(this, &SpotifyAPI::authenticated, this, &SpotifyAPI::saveRefreshToken);
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::refreshTokenReceived, this, &SpotifyAPI::saveRefreshToken);
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::refreshTokenReceived, [this](){ 
        if (m_firstTokenRefresh) {
            emit authenticated(); 
            m_firstTokenRefresh = false;
        }
    });

    // Delete saved token on error
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::errorThrown, m_tokenSaver, &SpotifyTokenSaver::deleteToken);

    connect(m_tokenSaver, &SpotifyTokenSaver::tokenRestored, this, &SpotifyAPI::tokenRestoredHandler);

    // Handle fetch previous and next playlists pages.
    connect(m_userPlaylist, &SpotifyPlaylist::fetchNextPage, this, &SpotifyAPI::fetchPrevNextPlaylistsPage);
    connect(m_userPlaylist, &SpotifyPlaylist::fetchPreviousPage, this, &SpotifyAPI::fetchPrevNextPlaylistsPage);
}

SpotifyAPI::~SpotifyAPI() 
{}

void SpotifyAPI::authenticate(bool saveToken)
{
    m_saveToken = saveToken;
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

SpotifyPlaylist* SpotifyAPI::userPlaylist()
{
    return m_userPlaylist;
}

void SpotifyAPI::updateProfile()
{
    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me"));
    QNetworkReply* reply = m_spotifyAuth->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        updateProfileHandler(reply);
    });
}

void SpotifyAPI::updateProfileHandler(QNetworkReply* reply)
{
    QByteArray replyData = reply->readAll();
    reply->deleteLater();

    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyData, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        emit error();
        qDebug() << "userProfile: Invalid JSON DATA";
        return;
    }

    QJsonObject rootObject = jsonDocument.object();
    if (rootObject.contains("error"))
    {
        emit error();
        qDebug() << "userProfile: Error";
        return;
    }

    QString username;

    const QString displayNameSection = "display_name";
    const QString idSection = "id";
    if (rootObject.contains(displayNameSection) && !rootObject.value(displayNameSection).isNull())
    {
        username = rootObject.value("display_name").toString();
    }
    else if (rootObject.contains(idSection) && !rootObject.value(idSection).isNull())
    {
        username = rootObject.value(idSection).toString();
    }

    QString country;

    const QString countrySection = "country";
    if (rootObject.contains(countrySection) && !rootObject.value(countrySection).isNull())
    {
        country = rootObject.value(countrySection).toString();
    }

    int followers = -1;

    const QString followersSection = "followers";
    if (rootObject.contains(followersSection) && !rootObject.value(followersSection).isNull())
    {
        QJsonObject followersObject = rootObject.value(followersSection).toObject();

        const QString totalFollowsersSection = "total";
        if (followersObject.contains(totalFollowsersSection) && !followersObject.value(totalFollowsersSection).isNull())
        {
            followers = followersObject.value(totalFollowsersSection).toInt(-1);
        }
    }

    m_userInfo->setUsername(username);
    m_userInfo->setCountry(country);
    m_userInfo->setFollowers(followers);
}

void SpotifyAPI::saveRefreshToken()
{
    if (!m_saveToken)
    {
        return m_tokenSaver->deleteToken();
    }

    const QString refreshToken = m_spotifyAuth->getRefreshToken();
    const QString accessToken = m_spotifyAuth->getAccessToken();
    const QString clientID = m_spotifyAuth->getClientID();
    const int tokenExpiration = m_spotifyAuth->getTokenExpiration();
    const long tokenRetrievalTime = m_spotifyAuth->getTokenRetrievevalTime();

    // If the token string is empty, something went wrong.
    if (refreshToken.isEmpty() || clientID.isEmpty() || accessToken.isEmpty()) return;

    const QString serializedTokenData =
        QString("%1;%2;%3;%4;%5")
            .arg(refreshToken, accessToken, clientID)
            .arg(tokenExpiration)
            .arg(tokenRetrievalTime);

    m_tokenSaver->writeToken(serializedTokenData);
}

void SpotifyAPI::restoreCredential()
{
    m_tokenSaver->readToken();
}

void SpotifyAPI::tokenRestoredHandler(const QString& token)
{
    const QStringList tokensList = token.split(';', Qt::SkipEmptyParts);

    if (tokensList.size() < 5) return;

    const QString refreshToken = tokensList.at(0);
    const QString accessToken = tokensList.at(1);
    const QString clientID = tokensList.at(2);
    
    bool isToIntConvertionSucceeded = false;
    const int tokenExpiration = tokensList.at(3).toInt(&isToIntConvertionSucceeded);

    if (!isToIntConvertionSucceeded) 
    {
        emit error();
        return;
    }

    const long tokenRetrievalTime = tokensList.at(4).toInt(&isToIntConvertionSucceeded);

    if (!isToIntConvertionSucceeded)
    {
        emit error();
        return;
    }

    m_saveToken = true;

    m_spotifyAuth->restoreRefreshToken(refreshToken, accessToken, clientID, tokenExpiration, tokenRetrievalTime);
}

void SpotifyAPI::fetchUserPlaylists()
{
    QUrlQuery query;
    query.addQueryItem("offset", "0");
    query.addQueryItem("limit", QString::number(SpotifyPlaylist::MAX_PER_PAGE));
    QNetworkRequest request(SpotifyPlaylist::SPOTIFY_USER_PLAYLIST_ENDPOINT + "?" + query.toString(QUrl::FullyEncoded));
    QNetworkReply* reply = m_spotifyAuth->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        m_userPlaylist->handleFetchResponse(reply);
    });
}

void SpotifyAPI::fetchPrevNextPlaylistsPage(const QUrl& url)
{
    if (!url.isValid()) return;

    QNetworkRequest request(url);
    QNetworkReply* reply = m_spotifyAuth->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply](){
        m_userPlaylist->handleFetchResponse(reply);
    });
}

void SpotifyAPI::displayPlaylistDetails(const QUrl& playlistHRef)
{
    if (!playlistHRef.isValid()) return;

    QNetworkRequest request(playlistHRef);
    QNetworkReply* reply = m_spotifyAuth->get(request);
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        this->handlePlaylistDetails(reply);
    });
}

void SpotifyAPI::handlePlaylistDetails(QNetworkReply* reply)
{
    const QByteArray responseData = reply->readAll();
    reply->deleteLater();

    SpotifyReceivedPlaylistElement* parsedPlaylist =
        new SpotifyReceivedPlaylistElement(responseData);
    
    if (parsedPlaylist->failed())
    {
        delete parsedPlaylist;
        qDebug() << "parse playlist failed";
        return;
    }

    emit receivedPlaylistDetails(parsedPlaylist);
}

void SpotifyAPI::logout()
{
    if (m_spotifyAuth->getAccessToken().isEmpty()) return;

    m_spotifyAuth->logout();
    m_userInfo->clearInfo();
    m_userPlaylist->clearData();
    m_saveToken = false;
    m_firstTokenRefresh = true;
    m_tokenSaver->deleteToken();
    emit loggedout();
}

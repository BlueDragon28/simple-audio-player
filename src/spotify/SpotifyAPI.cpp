#include "spotify/SpotifyAPI.h"
#include "spotify/SpotifyAuthorizationPKCE.h"
#include "spotify/SpotifyUserInfo.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>
#include <qnetworkreply.h>

SpotifyAPI::SpotifyAPI() : 
    QObject(),
    m_spotifyAuth(new SpotifyAuthorizationPKCE(this)),
    m_userInfo(new SpotifyUserInfo(this))
{
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::errorThrown, this, &SpotifyAPI::error);
    connect(m_spotifyAuth, &SpotifyAuthorizationPKCE::authenticated, this, &SpotifyAPI::authenticated);

    // When authenticated, request user information
    connect(this, &SpotifyAPI::authenticated, this, &SpotifyAPI::updateProfile);
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
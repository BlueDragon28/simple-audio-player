#include "spotify/SpotifyPlayer.h"
#include <QVariantMap>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkrequest.h>
#include <qnetworkreply.h>

SpotifyPlayer::SpotifyPlayer(SpotifyAuthorizationPKCE* spotifyAccess, QObject* parent) :
    QObject(parent),
    m_spotifyAccess(spotifyAccess)
{}

SpotifyPlayer::~SpotifyPlayer()
{}

void SpotifyPlayer::play(const QVariantMap& playArguments)
{
    if (!m_spotifyAccess || !m_spotifyAccess->isAuthenticated() ||
        !playArguments.contains("contex") ||
        !playArguments.contains("offset"))
    {
        return;
    }

    QVariant vContexUri = playArguments.value("contex");
    QVariant vOffset = playArguments.value("offset");

    if (!vContexUri.canConvert<QString>() || !vOffset.canConvert<int>() ||
        !vContexUri.convert(QMetaType::fromType<QString>()) ||
        !vOffset.convert(QMetaType::fromType<int>())) 
    {
        return;
    }

    const QString contexUri = vContexUri.toString();
    const int offset = vOffset.toInt();
    qDebug() << "contex_uri: " << contexUri;
    qDebug() << "offset: " << offset;

    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/play"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QVariantMap jsonOffset;
    jsonOffset["position"] = offset;

    QVariantMap jsonBody;
    jsonBody["context_uri"] = contexUri;
    jsonBody["offset"] = jsonOffset;

    const QByteArray bodyData =
        QJsonDocument::fromVariant(jsonBody).toJson(QJsonDocument::Compact);
    qDebug() << "jsonBodyData: " << bodyData;

    QNetworkReply* reply = 
        m_spotifyAccess->put(request, bodyData);

    if (!reply) return;

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        this->handlePlayResponse(reply);
    });
}

void SpotifyPlayer::handlePlayResponse(QNetworkReply* reply)
{
    if (!reply) return;

    const QByteArray response = reply->readAll();

    QJsonParseError error;
    QJsonDocument jsonDocument;

    if (!response.isEmpty())
    {
        jsonDocument = QJsonDocument::fromJson(response, &error);
    }

    if (error.error != QJsonParseError::NoError)
    {
        qDebug() << "Oups, failed to parse json: " << error.errorString();
        return;
    }

    if (jsonDocument.isObject())
    {
        const QJsonObject rootObject = jsonDocument.object();

        if (rootObject.contains("error"))
        {
            qDebug() << "response contains error object";
            return;
        }
    }

    emit isPlaying();
}

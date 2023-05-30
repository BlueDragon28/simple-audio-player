#include "spotify/SpotifyPlayer.h"
#include <QVariantMap>
#include <functional>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qnetworkrequest.h>
#include <qnetworkreply.h>
#include <qurlquery.h>

SpotifyPlayer::SpotifyPlayer(SpotifyAuthorizationPKCE* spotifyAccess, QObject* parent) :
    QObject(parent),
    m_spotifyAccess(spotifyAccess)
{}

SpotifyPlayer::~SpotifyPlayer()
{}

void SpotifyPlayer::play(const QVariantMap& playArguments)
{
    if (m_deviceID.isEmpty())
    {
        m_eventCallback.setCallback([this, playArguments]() {
            this->play(playArguments);
        });

        fetchAvailableDevices();
        return;
    }

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

    QUrlQuery urlQuery;
    urlQuery.addQueryItem("device_id", m_deviceID);

    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/play?" + urlQuery.query(QUrl::FullyEncoded)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QVariantMap jsonOffset;
    jsonOffset["position"] = offset;

    QVariantMap jsonBody;
    jsonBody["context_uri"] = contexUri;
    jsonBody["offset"] = jsonOffset;

    const QByteArray bodyData =
        QJsonDocument::fromVariant(jsonBody).toJson(QJsonDocument::Compact);

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
    reply->deleteLater();

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

void SpotifyPlayer::fetchAvailableDevices()
{
    if (!m_spotifyAccess || !m_spotifyAccess->isAuthenticated())
    {
        return;
    }

    m_deviceID = QString();

    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/devices"));
    QNetworkReply* reply = m_spotifyAccess->get(request);
    if (!reply) return;
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        this->handleAvailableDeviceResponse(reply);
    });
}

QString findMostAccurateDevice(const QJsonArray& devicesArray)
{
    if (devicesArray.isEmpty()) return QString();

    QString activeDeviceID;
    QString nonActiveDeviceID;

    for (const QJsonValue& deviceValue : devicesArray)
    {
        if (deviceValue.isNull() || !deviceValue.isObject()) continue;

        const QJsonObject deviceObject = deviceValue.toObject();

        if (deviceObject.isEmpty() || !deviceObject.contains("id") ||
            !deviceObject.contains("is_active") || !deviceObject.contains("is_restricted"))
        {
            continue;
        }

        const QString id = deviceObject.value("id").toString();
        const bool isActive = deviceObject.value("is_active").toBool();
        const bool isRestricted = deviceObject.value("is_restricted").toBool();

        if (id.isEmpty() || isRestricted) continue;

        if (isActive && activeDeviceID.isEmpty())
        {
            activeDeviceID = id;
        }
        else if (nonActiveDeviceID.isEmpty())
        {
            nonActiveDeviceID = id;
        }

        if (!activeDeviceID.isEmpty() && !nonActiveDeviceID.isEmpty()) break;
    }

    return !activeDeviceID.isEmpty() ? activeDeviceID :
        !nonActiveDeviceID.isEmpty() ? nonActiveDeviceID : QString();
}

void SpotifyPlayer::handleAvailableDeviceResponse(QNetworkReply* reply)
{
    if (!reply) 
    {
        return;
    }

    const QByteArray responseData = reply->readAll();
    reply->deleteLater();

    if (responseData.isEmpty())
    {
        return;
    }

    QJsonParseError error;
    const QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData, &error);

    if (error.error != QJsonParseError::NoError)
    {
        qDebug() << "Failed to parse response: " << error.errorString();
        return;
    }

    if (!jsonDocument.isObject())
    {
        qDebug() << "Invalid response";
        return;
    }

    const QJsonObject rootObject = jsonDocument.object();

    if (rootObject.isEmpty() || rootObject.contains("error") || !rootObject.contains("devices"))
    {
        qDebug() << "Invalid response or error!";
        return;
    }

    const QJsonValue devicesListValue = rootObject.value("devices");

    if (devicesListValue.isNull() || !devicesListValue.isArray())
    {
        qDebug() << "Invalid devices list";
        return;
    }

    const QJsonArray devicesArray = devicesListValue.toArray();

    if (devicesArray.isEmpty())
    {
        qDebug() << "no availables devices";
        return;
    }

    const QString deviceID = findMostAccurateDevice(devicesArray);
    if (deviceID.isEmpty()) return;

    m_deviceID = deviceID;

    m_eventCallback.call();
}

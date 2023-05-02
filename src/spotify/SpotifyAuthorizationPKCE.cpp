#include "spotify/SpotifyAuthorizationPKCE.h"
#include "spotify/SpotifyHttpCodeListener.h"
#include <QtNetworkAuth/qabstractoauth2.h>
#include <QtNetworkAuth/qoauth2authorizationcodeflow.h>
#include <bits/chrono.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <qcryptographichash.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnamespace.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>
#include <qurlquery.h>
#include <qnetworkreply.h>
#include <random>
#include <thread>

const uint16_t SpotifyAuthorizationPKCE::_listeningPort = 3000;

SpotifyAuthorizationPKCE::SpotifyAuthorizationPKCE(QObject* parent) :
    QObject(parent),
    m_accessManager(new QNetworkAccessManager(this)),
    m_httpCodeListener(new SpotifyHttpCodeListener(this)),

    m_codeVerifier(generateRandomString(128)),
    m_authorizationUrl(QUrl::fromUserInput("https://accounts.spotify.com/authorize")),
    m_redirectURL(QUrl::fromUserInput(QString("localhost:") + QString::number(_listeningPort))),
    m_accessTokenUrl(QUrl::fromUserInput("https://accounts.spotify.com/api/token")),
    m_scope("user-read-private user-read-email"),
    m_state(generateRandomString(16)),

    m_tokenExpiration(0),
    m_isAuthenticated(false)
{
    connect(this, &SpotifyAuthorizationPKCE::openBrowser, this, &SpotifyAuthorizationPKCE::openBrowserAndWaitForResponse);

    connect(m_httpCodeListener, &SpotifyHttpCodeListener::codeReceived, 
        this, &SpotifyAuthorizationPKCE::receiveCodeHandler);

    connect(this, &SpotifyAuthorizationPKCE::requestToken, this, &SpotifyAuthorizationPKCE::fetchToken);
}

SpotifyAuthorizationPKCE::~SpotifyAuthorizationPKCE()
{}

void SpotifyAuthorizationPKCE::prepareAuthorization()
{
    // const QByteArray codeChallenge = m_codeVerifier.toUtf8().toBase64();
    // const QByteArray sha256CodeChallenge = QCryptographicHash::hash(codeChallenge, QCryptographicHash::Sha256);
    const QByteArray sha256CodeVerifier = QCryptographicHash::hash(m_codeVerifier.toUtf8(), QCryptographicHash::Sha256);
    const QByteArray codeChallenge = 
        sha256CodeVerifier.toBase64(
            QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    QUrlQuery params;
    params.addQueryItem("client_id", m_clientID);
    params.addQueryItem("response_type", "code");
    params.addQueryItem("redirect_uri", m_redirectURL.toString(QUrl::FullyEncoded));
    params.addQueryItem("state", m_state);
    params.addQueryItem("scope", m_scope);
    params.addQueryItem("code_challenge_method", QString("S256"));
    params.addQueryItem("code_challenge", codeChallenge);

    QString queryParams = params.toString(QUrl::FullyEncoded);

    emit openBrowser(QUrl(m_authorizationUrl.toString(QUrl::FullyEncoded) + "?" + queryParams));
}

void SpotifyAuthorizationPKCE::openBrowserAndWaitForResponse(const QUrl& url)
{
    openUrlInBrowser(url);
    m_httpCodeListener->listen(_listeningPort);
}

void SpotifyAuthorizationPKCE::receiveCodeHandler(const QString& code, const QString& state)
{
    if (code.isEmpty() || state.isEmpty() || state.compare(m_state) != 0)
    {
        emit errorThrown();
        return;
    }

    m_code = code;
    emit requestToken();
}

void SpotifyAuthorizationPKCE::grant()
{
    if (!checkIfDataValidForAuthorization()) return;

    prepareAuthorization();
}

bool SpotifyAuthorizationPKCE::checkIfDataValidForAuthorization()
{
    if (m_clientID.isEmpty()) 
    {
        qDebug() << "client identifier cannot be empty!";
        emit errorThrown();
        return false;
    }

    if (!m_authorizationUrl.isValid())
    {
        qDebug() << "authorization url must be a valid url!";
        emit errorThrown();
        return false;
    }

    if (!m_redirectURL.isValid())
    {
        qDebug() << "redirect url must be a valid url!";
        emit errorThrown();
        return false;
    }

    if (!m_accessTokenUrl.isValid())
    {
        qDebug() << "access token url must be a valid url!";
        emit errorThrown();
        return false;
    }

    if (m_state.size() != 16)
    {
        qDebug() << "m_state must be 16 charaters!";
        emit errorThrown();
        return false;
    }

    if (m_code.size() > 0)
    {
        qDebug() << "You already have the code!";
        emit errorThrown();
        return false;
    }

    if (isAuthenticated()) 
    {
        qDebug() << "Already authenticated!";
        emit errorThrown();
        return false;
    }

    if (isTokenValid())
    {
        qDebug() << "Your token is valid!";
        emit errorThrown();
        return false;
    }

    return true;
}

void SpotifyAuthorizationPKCE::fetchToken()
{
    QNetworkRequest request(m_accessTokenUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery query;
    query.addQueryItem("grant_type", "authorization_code");
    query.addQueryItem("code", m_code);
    query.addQueryItem("redirect_uri", m_redirectURL.toString(QUrl::FullyEncoded));
    query.addQueryItem("client_id", m_clientID);
    query.addQueryItem("code_verifier", m_codeVerifier);

    QString body = query.query(QUrl::FullyEncoded);
    QByteArray bodyData = body.toUtf8();
    QNetworkReply* reply = m_accessManager->post(request, bodyData);
    connect(reply, &QNetworkReply::finished, [this, reply] () {
        tokenReceivedHandler(reply);
    });
}

void SpotifyAuthorizationPKCE::tokenReceivedHandler(QNetworkReply* reply)
{
    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonParseError error;
    QJsonDocument responseJSON = QJsonDocument::fromJson(responseData, &error);
    if (error.error != error.NoError) {
        emit errorThrown();
        return;
    }

    QJsonObject rootObject = responseJSON.object();

    if (!rootObject.contains("access_token"))
    {
        qDebug() << "invalid response!";
        emit errorThrown();
        return;
    }

    m_accessToken = rootObject.value("access_token").toString();
    m_refreshToken = rootObject.value("refresh_token").toString();
    m_tokenExpiration = rootObject.value("expires_in").toInt();
    m_tokenRetrievalTime = std::chrono::system_clock::now();

    m_isAuthenticated = true;
    emit authenticated();
}

QString SpotifyAuthorizationPKCE::generateRandomString(unsigned int size)
{
    if (size == 0) return "";

    static const QString validCharacters(u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    std::random_device randomDevice;
    std::uniform_int_distribution<int> distribution(0, validCharacters.size()-1);

    QString randomNumber;
    for (int i = 0; i < size; i++)
    {
        randomNumber += validCharacters.at(distribution(randomDevice));
    }

    return randomNumber;
}

void SpotifyAuthorizationPKCE::openUrlInBrowser(const QUrl& url)
{
    QString openBrowserCommand = 
#ifdef WIN32
        "start";
#else
        "xdg-open";
#endif 

    QString encodedUrl = QString::fromLocal8Bit(url.toEncoded());
    system((openBrowserCommand + " \"" + encodedUrl + "\"").toUtf8().constData());
}

bool SpotifyAuthorizationPKCE::isAuthenticated() const
{
    return m_isAuthenticated;
}

bool SpotifyAuthorizationPKCE::isTokenValid() const
{
    if (!isAuthenticated()) return false;

    auto now = std::chrono::system_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - m_tokenRetrievalTime);
    return duration.count() < m_tokenExpiration;
}

void SpotifyAuthorizationPKCE::setClientID(const QString& clientID)
{
    m_clientID = clientID;
}

void SpotifyAuthorizationPKCE::refreshToken()
{
    if (isTokenValid()) return;

    QNetworkRequest refreshRequest(m_accessTokenUrl);
    refreshRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery query;
    query.addQueryItem("grant_type", "refresh_token");
    query.addQueryItem("refresh_token", m_refreshToken);
    query.addQueryItem("client_id", m_clientID);

    const QByteArray body = query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply* reply = m_accessManager->post(refreshRequest, body);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        refreshTokenReceivedHandler(reply);
    });
}

QString SpotifyAuthorizationPKCE::getRefreshToken() const
{
    return m_refreshToken;
}

QString SpotifyAuthorizationPKCE::getAccessToken() const
{
    return m_accessToken;
}

QString SpotifyAuthorizationPKCE::getClientID() const
{
    return m_clientID;
}

int SpotifyAuthorizationPKCE::getTokenExpiration() const
{
    return m_tokenExpiration;
}

long SpotifyAuthorizationPKCE::getTokenRetrievevalTime() const
{
    std::time_t time = std::chrono::system_clock::to_time_t(m_tokenRetrievalTime);
    return time;
}

void SpotifyAuthorizationPKCE::refreshTokenReceivedHandler(QNetworkReply* networkReply)
{
    const QByteArray responseData = networkReply->readAll();
    networkReply->deleteLater();

    QJsonParseError jsonError;
    QJsonDocument JSONDocument = QJsonDocument::fromJson(responseData, &jsonError);

    if (jsonError.error != QJsonParseError::NoError)
    {
        m_isAuthenticated = false;
        emit errorThrown();
        return;
    }

    QJsonObject rootObject = JSONDocument.object();

    if (!rootObject.contains("access_token"))
    {
        qDebug() << "Invalid refresh token!";
        qDebug() << rootObject;
        emit errorThrown();
        return;
    }

    m_accessToken = rootObject.value("access_token").toString();
    m_tokenExpiration = rootObject.value("expires_in").toInt();
    m_tokenRetrievalTime = std::chrono::system_clock::now();

    if (rootObject.contains("refresh_token"))
    {
        m_refreshToken = rootObject.value("refresh_token").toString();
    }

    emit refreshTokenReceived();
}

void SpotifyAuthorizationPKCE::addAuthorizationHeader(QNetworkRequest* request) const 
{
    QByteArray authorization(u8"Authorization");
    QByteArray bearer(u8"Bearer ");
    bearer.append(m_accessToken.toUtf8());
    request->setRawHeader(authorization, bearer);
}

bool SpotifyAuthorizationPKCE::refreshTokenIfNeeded()
{
    if (!isAuthenticated()) return false;

    if (!isTokenValid())
    {
        refreshToken();

        while (isAuthenticated() && !isTokenValid()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        if (!isAuthenticated())
        {
            return false;
        }
    }

    return true;
}

QNetworkReply* SpotifyAuthorizationPKCE::get(const QNetworkRequest& userRequest)
{
    if (!refreshTokenIfNeeded()) return nullptr;

    QNetworkRequest request(userRequest); 
    addAuthorizationHeader(&request);

    QNetworkReply* reply = m_accessManager->get(request);
    return reply;
}

void SpotifyAuthorizationPKCE::restoreRefreshToken(
    const QString& refreshToken, 
    const QString& accessToken,
    const QString& clientID,
    int tokenExpiration,
    long tokenRetrievalTime)
{
    if (refreshToken.isEmpty()) return;

    m_accessToken = accessToken;
    m_tokenExpiration = tokenExpiration;
    m_tokenRetrievalTime = std::chrono::system_clock::from_time_t(tokenRetrievalTime);
    m_refreshToken = refreshToken;
    m_clientID = clientID;
    m_isAuthenticated = true;

    if (!isTokenValid())
    {
        this->refreshToken();
    }
    else 
    {
        emit refreshTokenReceived();
    }
}
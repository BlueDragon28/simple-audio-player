#ifndef SIMPLEAUDIOPLAYER_SPOTIFYAUTHORIZATIONPKCE_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYAUTHORIZATIONPKCE_H_

#include "spotify/SpotifyHttpCodeListener.h"
#include <chrono>
#include <ctime>
#include <qnetworkaccessmanager.h>
#include <qobject.h>
#include <qmap.h>
#include <qvariant.h>

/*
Authorisation and authotentification to Spotify API using the authorization PKCE.
*/

class SpotifyAuthorizationPKCE : public QObject 
{
    Q_OBJECT

public:
    SpotifyAuthorizationPKCE(QObject* parent = nullptr);
    virtual ~SpotifyAuthorizationPKCE();

    bool isAuthenticated() const;
    void setClientID(const QString& clientID);

public slots:
    void grant();

signals:
    void openBrowser(const QUrl& url);
    void requestToken();
    void authenticated();

private:
    void prepareAuthorization();
    void openBrowserAndWaitForResponse(const QUrl& url);
    void receiveCodeHandler(const QString& code, const QString& state);
    bool checkIfDataValidForAuthorization() const;
    void fetchToken();
    void tokenReceivedHandler(QNetworkReply* reply);

    static QString generateRandomString(unsigned int size);

    QNetworkAccessManager* m_accessManager;
    SpotifyHttpCodeListener* m_httpCodeListener;

    // Retrieve token specific data
    QString m_clientID;
    const QString m_codeVerifier;
    QString m_code;
    const QUrl m_authorizationUrl;
    const QUrl m_redirectURL;
    const QUrl m_accessTokenUrl;
    QString m_scope;
    const QString m_state;

    // Token data
    QString m_accessToken;
    QString m_refreshToken;
    unsigned int m_tokenExpiration;
    std::chrono::system_clock::time_point m_tokenRetrievalTime;
    bool m_isAuthenticated;

    static const uint16_t _listeningPort;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYAUTHORIZATIONPKCE_H_

#ifndef SIMPLEAUDIOPLAYER_SPOTIFYHTTPCODELISTENER_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYHTTPCODELISTENER_H_

#include "httplib.h"
#include <thread>
#include <qobject.h>

class SpotifyHttpCodeListener : public QObject
{
    Q_OBJECT 

public:
    SpotifyHttpCodeListener(QObject* parent = nullptr);
    virtual ~SpotifyHttpCodeListener();

public slots:
    bool listen(int port);

signals:
    void codeReceived(const QString& code, const QString& state);
    void codeInvalid();
    void stopServer();

private:
    void codeRouteRequest(const QString& code, const QString& state);
    void stopServerHandler();

    std::thread m_serverThread;
    httplib::Server* m_localServer;
    bool m_handleRequest;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYHTTPCODELISTENER_H_


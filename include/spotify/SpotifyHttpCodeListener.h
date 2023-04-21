#ifndef SIMPLEAUDIOPLAYER_SPOTIFYHTTPCODELISTENER_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYHTTPCODELISTENER_H_

#include <QtHttpServer/qhttpserver.h>
#include <qobject.h>

class SpotifyHttpCodeListener : public QObject
{
    Q_OBJECT 

public:
    SpotifyHttpCodeListener(QObject* parent = nullptr);
    virtual ~SpotifyHttpCodeListener();

public slots:
    bool listen(int port);
    void stopListening();

signals:
    void codeReceived(const QString& code, const QString& state);
    void codeInvalid();

private:
    void codeRouteRequest(const QString& code, const QString& state);

    QHttpServer* m_localServer;
    bool m_handleRequest;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYHTTPCODELISTENER_H_


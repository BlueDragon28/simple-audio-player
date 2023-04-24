#ifndef SIMPLEAUDIOPLAYER_SPOTIFYTOKENSAVER_H_
#define SIMPLEAUDIOPLAYER_SPOTIFYTOKENSAVER_H_

#include <qobject.h>
#include <qt6keychain/keychain.h>

class SpotifyTokenSaver : public QObject
{
    Q_OBJECT

public:
    SpotifyTokenSaver(QObject* parent = nullptr);
    ~SpotifyTokenSaver();

public slots:
    void readToken();
    void writeToken(const QString& token);
    void deleteToken();

signals:
    void tokenRestored(const QString& token);

private:
    void tokenReadedHandler();
    void tokenWritedHandler();
    void tokenDeletedHandler();

    QKeychain::ReadPasswordJob m_readTokenJob;
    QKeychain::WritePasswordJob m_writeTokenJob;
    QKeychain::DeletePasswordJob m_deleteTokenJob;
};

#endif // SIMPLEAUDIOPLAYER_SPOTIFYTOKENSAVER_H_
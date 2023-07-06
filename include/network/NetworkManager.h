#ifndef SIMPLEAUDIOPLAYER_NETWORKMANAGER_H_
#define SIMPLEAUDIOPLAYER_NETWORKMANAGER_H_

#include <QtNetwork/qnetworkaccessmanager.h>

/*
A simple static class to hold an instance of a access manager
*/

class NetworkManager
{
    NetworkManager() = delete;
    NetworkManager(const NetworkManager&) = delete;
    ~NetworkManager() = delete;

public:
    static QNetworkAccessManager* accessManager();

    static void createAccessManager();
    static void destroyAccessManager();

private:
    static QNetworkAccessManager* _accessManager;
};

#endif // SIMPLEAUDIOPLAYER_NETWORKMANAGER_H_


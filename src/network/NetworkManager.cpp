#include "network/NetworkManager.h"
#include <qnetworkaccessmanager.h>

QNetworkAccessManager* NetworkManager::_accessManager = nullptr;

QNetworkAccessManager* NetworkManager::accessManager()
{
    return _accessManager;
}

void NetworkManager::createAccessManager()
{
    _accessManager = new QNetworkAccessManager();
}

void NetworkManager::destroyAccessManager()
{
    delete _accessManager;
    _accessManager = nullptr;
}
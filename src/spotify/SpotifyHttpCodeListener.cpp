#include "spotify/SpotifyHttpCodeListener.h"
#include <QtHttpServer/qhttpserver.h>
#include <QtHttpServer/qhttpserverrequest.h>
#include <qhostaddress.h>

SpotifyHttpCodeListener::SpotifyHttpCodeListener(QObject* parent) :
    QObject(parent),
    m_localServer(nullptr),
    m_handleRequest(true)
{}

SpotifyHttpCodeListener::~SpotifyHttpCodeListener()
{
}

bool SpotifyHttpCodeListener::listen(int port)
{
    if (port == 0) return false;

    if (m_localServer)
    {
        m_localServer->deleteLater();
        m_localServer = nullptr;
    }

    m_localServer = new QHttpServer(this);
    m_localServer->route("/", [this](const QHttpServerRequest& request) 
    {
        if (!m_handleRequest)
        {
            return QString("Request already processed");
        }

        QUrlQuery query = request.query();

        if (!query.hasQueryItem("code") || !query.hasQueryItem("state"))
        {
            emit codeInvalid();
            return QString("Oups: something went wrong!");
        }

        const QString& code = query.queryItemValue("code");
        const QString& state = query.queryItemValue("state");

        this->codeRouteRequest(code, state);

        return QString("Return to the application.");
    });

    int listeningPort = m_localServer->listen(QHostAddress::Any, port);
    m_handleRequest = true;

    return listeningPort == port;
}

void SpotifyHttpCodeListener::stopListening()
{
    if (!m_localServer) return;

    m_localServer->deleteLater();
}

void SpotifyHttpCodeListener::codeRouteRequest(const QString& code, const QString& state)
{
    if (code.isEmpty() || state.isEmpty())
    {
        emit codeInvalid();
        return;
    }

    m_handleRequest = false;
    emit codeReceived(code, state);
}
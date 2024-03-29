#include "spotify/SpotifyHttpCodeListener.h"
#include "httplib.h"
#include <qhostaddress.h>
#include <thread>

SpotifyHttpCodeListener::SpotifyHttpCodeListener(QObject* parent) :
    QObject(parent),
    m_localServer(nullptr),
    m_handleRequest(true)
{
    connect(this, &SpotifyHttpCodeListener::stopServer, this, &SpotifyHttpCodeListener::stopServerHandler);
}

SpotifyHttpCodeListener::~SpotifyHttpCodeListener()
{
    stopServerHandler();
}

bool SpotifyHttpCodeListener::listen(int port)
{
    if (port == 0) return false;

    stopServerHandler();

    m_localServer = new httplib::Server();

    m_localServer->Get(u8"/", [this](const httplib::Request& req, httplib::Response& res) 
    {
        if (!m_handleRequest)
        {
            res.set_content("Request already processed!", "text/plain");
            return;
        }

        if (!req.has_param(u8"code") || !req.has_param(u8"state"))
        {
            emit codeInvalid();
            res.set_content("Oups: something went wrong!", "text/plain");
            return;
        }

        const std::string code = req.get_param_value(u8"code");
        const std::string state = req.get_param_value(u8"state");

        res.set_content("Return to the application", "text/plain");

        this->codeRouteRequest(
            QString::fromUtf8(code.c_str()), 
            QString::fromUtf8(state.c_str()));

    });

    bool result = m_localServer->bind_to_port(u8"localhost", port);

    m_serverThread = std::thread([this]() {
        m_localServer->listen_after_bind();
    });

    return result;
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

void SpotifyHttpCodeListener::stopServerHandler()
{
    if (!m_localServer) 
    {
        return;
    }

    if (m_localServer->is_running())
    {
        m_localServer->stop();
    }

    if (m_serverThread.joinable())
    {
        m_serverThread.join();
    }

    delete m_localServer;
    m_localServer = nullptr;
}

void SpotifyHttpCodeListener::resetRequestHandler()
{
    m_handleRequest = true;
    stopServerHandler();
}

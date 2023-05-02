#include "spotify/SpotifyHttpCodeListener.h"
#include "httplib.h"
#include <qdebug.h>
#include <qhostaddress.h>
#include <thread>

SpotifyHttpCodeListener::SpotifyHttpCodeListener(QObject* parent) :
    QObject(parent),
    m_localServer(nullptr),
    m_handleRequest(true)
{}

SpotifyHttpCodeListener::~SpotifyHttpCodeListener()
{
    if (m_localServer) 
    {
        m_localServer->stop();
        if (m_serverThread.joinable())
        {
            m_serverThread.join();
        }
        delete m_localServer;
    }
}

bool SpotifyHttpCodeListener::listen(int port)
{
    if (port == 0) return false;

    if (m_localServer) 
    {
        m_localServer->stop();
        if (m_serverThread.joinable())
        {
            m_serverThread.join();
        }
        delete m_localServer;
        m_localServer = nullptr;
    }

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


        this->codeRouteRequest(
            QString::fromUtf8(code.c_str()), 
            QString::fromUtf8(state.c_str()));

        res.set_content("Return to the application", "text/plain");
    });

    bool result = m_localServer->bind_to_port(u8"localhost", port);

    m_serverThread = std::thread([this]() {
        qDebug() << "before listen call";
        m_localServer->listen_after_bind();
        qDebug() << "after listen call";
    });

    if (result) {
        qDebug() << "local server bind to port: " << port;
    } else {
        qDebug() << "failed to bind to port: " << port;
    }

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
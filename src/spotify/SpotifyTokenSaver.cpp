#include "spotify/SpotifyTokenSaver.h"
#include <qdebug.h>
#include <qt6keychain/keychain.h>

const QString storingKey = "Et6Q3FRspERa4HoA2NwZDGCQkFsvmw";

SpotifyTokenSaver::SpotifyTokenSaver(QObject* parent) :
    QObject(parent),
    m_readTokenJob("simpleaudioplayer-app"),
    m_writeTokenJob("simpleaudioplayer-app"),
    m_deleteTokenJob("simpleaudioplayer-app")
{
    m_readTokenJob.setAutoDelete(false);
    m_writeTokenJob.setAutoDelete(false);
    m_deleteTokenJob.setAutoDelete(false);

    connect(&m_readTokenJob, &QKeychain::ReadPasswordJob::finished, this, &SpotifyTokenSaver::tokenReadedHandler);
    connect(&m_writeTokenJob, &QKeychain::WritePasswordJob::finished, this, &SpotifyTokenSaver::tokenWritedHandler);
    connect(&m_deleteTokenJob, &QKeychain::DeletePasswordJob::finished, this, &SpotifyTokenSaver::tokenDeletedHandler);
}

SpotifyTokenSaver::~SpotifyTokenSaver()
{}

void SpotifyTokenSaver::writeToken(const QString& token)
{
    m_writeTokenJob.setKey(storingKey);

    m_writeTokenJob.setTextData(token);
    m_writeTokenJob.start();
}

void SpotifyTokenSaver::readToken()
{
    m_readTokenJob.setKey(storingKey);

    m_readTokenJob.start();
}

void SpotifyTokenSaver::deleteToken()
{
    m_deleteTokenJob.setKey(storingKey);
    m_deleteTokenJob.start();
}

void SpotifyTokenSaver::tokenReadedHandler()
{
    if (m_readTokenJob.error() != QKeychain::Error::NoError)
    {
        qDebug() << "Oups: something went wrong while reading the token!";
        return;
    }

    emit tokenRestored(m_readTokenJob.textData());
}

void SpotifyTokenSaver::tokenWritedHandler()
{
    if (m_writeTokenJob.error() != QKeychain::Error::NoError)
    {
        qDebug() << "Oups: something went wrong while saving the token!";
    }
}

void SpotifyTokenSaver::tokenDeletedHandler()
{
    if (m_writeTokenJob.error() != QKeychain::Error::NoError)
    {
        qDebug() << "Oups: failed to delete token";
    }
}
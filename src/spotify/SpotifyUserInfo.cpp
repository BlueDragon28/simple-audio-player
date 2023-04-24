#include "spotify/SpotifyUserInfo.h"

SpotifyUserInfo::SpotifyUserInfo(QObject* parent) :
    QObject(parent),
    m_username("Unknown"),
    m_country("NA"),
    m_followers(0)
{}

SpotifyUserInfo::~SpotifyUserInfo()
{}

QString SpotifyUserInfo::username() const
{
    return m_username;
}

QString SpotifyUserInfo::country() const
{
    return m_country;
}

int SpotifyUserInfo::followers() const
{
    return m_followers;
}

void SpotifyUserInfo::setUsername(const QString& username)
{
    if (username.compare(m_username) == 0) return;

    m_username = username.size() > 0 ? username : "Unknown";

    emit usernameChanged();
}

void SpotifyUserInfo::setCountry(const QString& country)
{
    if (country.compare(m_country) == 0) return;

    m_country = country.size() > 0 ? country : "NA";

    emit countryChanged();
}

void SpotifyUserInfo::setFollowers(int followers)
{
    if (followers == m_followers) return ;

    m_followers = followers >= 0 ? followers : 0;

    emit followersChanged();
}
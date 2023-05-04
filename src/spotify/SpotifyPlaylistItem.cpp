#include "spotify/SpotifyPlaylistItem.h"

SpotifyPlaylistItem::SpotifyPlaylistItem(const QString& title, const QUrl& imageUrl, QObject* parent) :
    QObject(parent),
    m_title(title),
    m_imageUrl(imageUrl)
{}

SpotifyPlaylistItem::~SpotifyPlaylistItem()
{}

QString SpotifyPlaylistItem::title() const
{
    return m_title;
}

QUrl SpotifyPlaylistItem::imageUrl() const
{
    return m_imageUrl;
}

bool SpotifyPlaylistItem::operator==(const SpotifyPlaylistItem& other) const
{
    return other.m_title.compare(m_title) == 0 && other.m_imageUrl == m_imageUrl;
}

SpotifyPlaylistItem::operator QString() const
{
    return QString("{\"title\":\"%1\",\"imageUrl\":\"%2\"}")
        .arg(m_title, m_imageUrl.toString());
}

void SpotifyPlaylistItem::setTitle(const QString& title)
{
    if (title == m_title) return;

    m_title = title;
    emit titleChanged();
}

void SpotifyPlaylistItem::setImageUrl(const QUrl& imageUrl)
{
    if (imageUrl == m_imageUrl) return;

    m_imageUrl = imageUrl;
    emit imageUrlChanged();
}
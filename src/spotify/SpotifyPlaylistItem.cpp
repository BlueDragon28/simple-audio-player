#include "spotify/SpotifyPlaylistItem.h"

SpotifyPlaylistItem::SpotifyPlaylistItem(const QString& title, const QUrl& imageUrl, const QUrl& href, const QString& id, QObject* parent) :
    QObject(parent),
    m_title(title),
    m_imageUrl(imageUrl),
    m_href(href),
    m_id(id)
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

QUrl SpotifyPlaylistItem::href() const
{
    return m_href;
}

QString SpotifyPlaylistItem::id() const
{
    return m_id;
}

bool SpotifyPlaylistItem::operator==(const SpotifyPlaylistItem& other) const
{
    return other.m_title.compare(m_title) == 0 && 
        other.m_imageUrl == m_imageUrl &&
        other.m_href == m_href &&
        other.m_id == m_id;
}

SpotifyPlaylistItem::operator QString() const
{
    return QString("{\"title\":\"%1\",\"imageUrl\":\"%2\",\"href\":\"%3\",\"id\":\"%4\"}")
        .arg(m_title, m_imageUrl.toString(), m_href.toString(), m_id);
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

void SpotifyPlaylistItem::setHRef(const QUrl& href)
{
    if (href == m_href) return;

    m_href = href;
    emit hrefChanged();
}

void SpotifyPlaylistItem::setId(const QString& id)
{
    if (id == m_id) return;

    m_id = id;
    emit idChanged();
}
#include "spotify/SpotifyPlaylist.h"
#include "SpotifyPlaylist.h"
#include "SpotifyPlaylistItem.h"
#include <cmath>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qnetworkreply.h>
#include <qobject.h>

const int SpotifyPlaylist::MAX_PER_PAGE = 10;
const QString SpotifyPlaylist::SPOTIFY_USER_PLAYLIST_ENDPOINT = "https://api.spotify.com/v1/me/playlists";
const int SpotifyPlaylist::LIMIT_IMAGE_HEIGHT = 300;

SpotifyPlaylist::SpotifyPlaylist(QObject* parent) :
    QObject(parent),
    m_total(0),
    m_pageNumber(0),
    m_totalPages(0),
    m_numberOnPage(0),
    m_hasNext(false),
    m_hasPrevious(false)
{}

SpotifyPlaylist::~SpotifyPlaylist()
{}

int SpotifyPlaylist::total() const
{
    return m_total;
}

void SpotifyPlaylist::setTotal(int total)
{
    if (total == m_total) return;

    m_total = total;
    emit totalChanged();
    qDebug() << "playlist total: " << total;
}

int SpotifyPlaylist::pageNumber() const
{
    return m_pageNumber;
}

void SpotifyPlaylist::setPageNumber(int pageNumber)
{
    if (pageNumber == m_pageNumber) return;

    m_pageNumber = pageNumber;
    emit pageNumberChanged();
    qDebug() << "playlist page number: " << pageNumber;
}

int SpotifyPlaylist::numberOnPage() const
{
    return m_numberOnPage;
}

void SpotifyPlaylist::setNumberOnPage(int numberOnPage)
{
    if (numberOnPage == m_numberOnPage) return;

    m_numberOnPage = numberOnPage;
    emit numberOnPageChanged();
    qDebug() << "playlist number on page: " << numberOnPage;
}

int SpotifyPlaylist::totalPages() const
{
    return m_totalPages;
}

void SpotifyPlaylist::setTotalPages(int totalPages)
{
    if (totalPages == m_totalPages) return;

    m_totalPages = totalPages;
    emit totalPagesChanged();
    qDebug() << "playlist total pages: " << totalPages;
}

QList<QObject*>* SpotifyPlaylist::playlists()
{
    return &m_playlists;
}

void SpotifyPlaylist::setPlaylists(const QList<QObject*>& playlists)
{
    if (playlists == m_playlists) return;

    destroyPlaylistsObjects();
    m_playlists = playlists;
    emit playlistsChanged();

    QString str("[");
    for (const QObject* item : m_playlists)
    {
        str += *static_cast<const SpotifyPlaylistItem*>(item) + ",";
    }
    str += "]";
    qDebug() << str;
}

void SpotifyPlaylist::destroyPlaylistsObjects()
{
    for (QObject* object : m_playlists)
    {
        delete object;
    }
}

QUrl SpotifyPlaylist::next() const
{
    return m_next;
}

void SpotifyPlaylist::setNext(const QString& nextUrl)
{
    m_next = QUrl(nextUrl);
    setHasNext(m_next);
    qDebug() << "playlist next url: " << nextUrl;
}

bool SpotifyPlaylist::hasNext() const
{
    return m_hasNext;
}

void SpotifyPlaylist::setHasNext(const QUrl& url)
{
    const bool hasNext = url.isValid();
    if (hasNext == m_hasNext) return;

    m_hasNext = hasNext;
    emit hasNextChanged();
}

QUrl SpotifyPlaylist::previous() const
{
    return m_previous;
}

void SpotifyPlaylist::setPrevious(const QString& previousUrl)
{
    m_previous = QUrl(previousUrl);
    setHasPrevious(m_previous);
    qDebug() << "playlist previous url: " << previousUrl;
}

bool SpotifyPlaylist::hasPrevious() const
{
    return m_hasPrevious;
}

void SpotifyPlaylist::setHasPrevious(const QUrl& url)
{
    const bool hasPrevious = url.isValid();
    if (hasPrevious == m_hasPrevious) return;
    m_hasPrevious = hasPrevious;
    emit hasPreviousChanged();
}

void SpotifyPlaylist::handleFetchResponse(QNetworkReply* reply)
{
    const QByteArray data = reply->readAll();
    reply->deleteLater();
    qDebug() << "user playlists list response: " << data;

    QJsonParseError parseError;
    const QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        emit error();
        qDebug() << "fetchUserPlaylists: Invalid JSON DATA";
        return;
    }

    if (!jsonDocument.isObject())
    {
        emit error();
        qDebug() << "fetchUserPlaylists: Invalid JSON";
        return;
    }

    const QJsonObject rootObject = jsonDocument.object();
    if (rootObject.contains("error"))
    {
        emit error();
        qDebug() << "fetchUserPlaylists: oups: something went wrong";
        return;
    }

    if (!rootObject.contains("total"))
    {
        emit error();
        qDebug() << "fetchUserPlaylists: total not found";
        return;
    }

    const int total = rootObject.value("total").toInt(-1);
    if (total == -1)
    {
        emit error();
        qDebug() << "fetchUserPlaylists: invalid total value";
        return;
    }

    if (!rootObject.contains("limit"))
    {
        emit error();
        qDebug() << "fetchUserPlaylists: no limit found";
        return;
    }

    const int limit = rootObject.value("limit").toInt(-1);

    if (limit == -1)
    {
        emit error();
        qDebug() << "fetchUserPlaylists: invalid limit";
        return;
    }

    if (!rootObject.contains("offset"))
    {
        emit error();
        qDebug() << "fetchUserPlaylists: no offset found";
        return;
    }

    const int offset = rootObject.value("offset").toInt(-1);

    if (offset == -1)
    {
        emit error();
        qDebug() << "fetchUserPlaylists: invalid offset";
        return;
    }

    if (!rootObject.contains("next"))
    {
        emit error();
        qDebug() << "fetchUserPlaylists: no next url found";
        return;
    }

    const QUrl nextUrl =
        rootObject.value("next").toString();

    if (!rootObject.contains("previous"))
    {
        emit error();
        qDebug() << "fetchUserPlaylists: no previous url found";
        return;
    }

    const QUrl previousUrl =
        rootObject.value("previous").toString();

    if (!rootObject.contains("items"))
    {
        emit error();
        qDebug() << "fetchUserPlaylists: no items array found";
        return;
    }

    const QJsonValue valueItems = rootObject.value("items");
    if (!valueItems.isArray())
    {
        emit error();
        qDebug() << "fetchUserPlaylists: items is not an array";
        return;
    }

    const QJsonArray items = rootObject.value("items").toArray();
    QList<QObject*> playlistsItems;

    for (const QJsonValue& item : items)
    {
        SpotifyPlaylistItem* playlistsItem = 
            parsePlaylistJSONItem(item);
        
        if (!playlistsItem) continue;
        playlistsItems.append(playlistsItem);
    }

    const int maxPageNumber = 
        std::max(1, (int)std::ceil((double)total / (double)limit));
    const int currentPageNumber =
        std::max(1, (int)std::ceil((double)(offset+playlistsItems.size()) / (double)limit));

    setTotal(total);
    setNext(nextUrl.toString(QUrl::FullyEncoded));
    setPrevious(previousUrl.toString(QUrl::FullyEncoded));
    setPlaylists(playlistsItems);
    setPageNumber(currentPageNumber);
    setTotalPages(maxPageNumber);
    emit playlistLoaded();
}

SpotifyPlaylistItem* SpotifyPlaylist::parsePlaylistJSONItem(const QJsonValue& itemValue)
{
    if (!itemValue.isObject())
    {
        return nullptr;
    }

    const QJsonObject object = itemValue.toObject();

    if (!object.contains("name"))
    {
        return nullptr;
    }

    const QString name = object.value("name").toString();

    if (!object.contains("images"))
    {
        return nullptr;
    }

    const QJsonValue imagesArrayValue = object.value("images");
    const QUrl imageUrl = parseImagesUrls(imagesArrayValue);

    return new SpotifyPlaylistItem(name, imageUrl, this);
}

QUrl SpotifyPlaylist::parseImagesUrls(const QJsonValue& imagesArrayValue)
{
    QUrl imageUrl;

    if (!imagesArrayValue.isArray())
    {
        return imageUrl;
    }

    const QJsonArray imagesArray = imagesArrayValue.toArray();

    if (imagesArray.isEmpty())
    {
        return imageUrl;
    }

    int maxHeight = 0;

    for (const QJsonValue& imageValue : imagesArray)
    {
        if (!imageValue.isObject()) continue;

        const QJsonObject imageObject = imageValue.toObject();

        if (!imageObject.contains("height")) continue;
        if (!imageObject.contains("url")) continue;

        const int height = imageObject.value("height").toInt(-1);
        if (height == -1) continue;

        if (height <= maxHeight || height > LIMIT_IMAGE_HEIGHT) continue;

        const QString strUrl = imageObject.value("url").toString();
        if (!strUrl.isEmpty()) continue;

        maxHeight = height;
        imageUrl = strUrl;
    }

    if (!imageUrl.isValid())
    {
        const QJsonValue firstImage = imagesArray.at(0);
        if (!firstImage.isObject())
        {
            return imageUrl;
        }

        const QJsonObject imageObject = firstImage.toObject();

        if (!imageObject.contains("url"))
        {
            return imageUrl;
        }

        const QString strUrl = imageObject.value("url").toString();
        if (strUrl.isEmpty())
        {
            return imageUrl;
        }

        imageUrl = strUrl;
    }

    return imageUrl;
}

void SpotifyPlaylist::fetchNext()
{
    if (!m_hasNext || !m_next.isValid()) return;

    emit fetchNextPage(m_next);
}

void SpotifyPlaylist::fetchPrevious()
{
    if (!m_hasPrevious || !m_previous.isValid()) return;

    emit fetchPreviousPage(m_previous);
}
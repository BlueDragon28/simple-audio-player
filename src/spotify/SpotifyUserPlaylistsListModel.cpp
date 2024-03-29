#include "SpotifyUserPlaylistsListModel.h"
#include "CoverCache.h"
#include "SpotifyPlaylist.h"
#include "SpotifyPlaylistItem.h"
#include <cstdint>
#include <qabstractitemmodel.h>
#include <qvariant.h>

SpotifyUserPlaylistsListModel::SpotifyUserPlaylistsListModel(QObject* parent) :
    QAbstractListModel(parent),
    m_playlistApiInfo(nullptr)
{
    connect(
        CoverCache::instance(), 
        &CoverCache::imageReceived,
        this,
        &SpotifyUserPlaylistsListModel::coverImageReceived);
}

SpotifyUserPlaylistsListModel::~SpotifyUserPlaylistsListModel()
{}

int SpotifyUserPlaylistsListModel::rowCount(const QModelIndex& parent) const
{
    return m_items.size();
}

QVariant SpotifyUserPlaylistsListModel::data(const QModelIndex& index, int role) const
{
    if (role != TITLE && role != IMAGE_URL && role != HREF && role != ID) return QVariant();

    const int i = index.row();
    QVariant vItem = m_items.at(i);

    if (!vItem.canConvert<PlaylistItem>() || !vItem.convert(QMetaType::fromType<PlaylistItem>()))
        return QVariant();
    
    const PlaylistItem item = qvariant_cast<PlaylistItem>(vItem);

    if (role == TITLE)
    {
        return item.title;
    } 
    else if (role == IMAGE_URL)
    {
        return item.imageUrl;
    }
    else if (role == HREF)
    {
        return item.href;
    }
    else if (role == ID) 
    {
        return item.id;
    }

    return QVariant();
}

QHash<int, QByteArray> SpotifyUserPlaylistsListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TITLE] = "title";
    roles[IMAGE_URL] = "imageUrl";
    roles[HREF] = "href";
    roles[ID] = "id";
    return roles;
}

SpotifyPlaylist* SpotifyUserPlaylistsListModel::playlistsAPI() const
{
    return m_playlistApiInfo;
}

void SpotifyUserPlaylistsListModel::setPlaylistsAPI(SpotifyPlaylist* playlists)
{
    if (playlists == m_playlistApiInfo) return;

    m_playlistApiInfo = playlists;
    emit playlistsAPIChanged();

    if (!m_playlistApiInfo) return;

    connect(m_playlistApiInfo, &SpotifyPlaylist::playlistLoaded, this, &SpotifyUserPlaylistsListModel::retrievePlaylists);
    retrievePlaylists();
}

void SpotifyUserPlaylistsListModel::retrievePlaylists()
{
    if (!m_playlistApiInfo) return;

    if (m_items.size() > 0)
    {
        beginRemoveRows(QModelIndex(), 0, m_items.size()-1);
        m_items.clear();
        endRemoveRows();
    }

    const QList<QObject*>* playlists = m_playlistApiInfo->playlists();

    if (!playlists || playlists->isEmpty()) return;

    for (const QObject* obj : *playlists) 
    {
        const SpotifyPlaylistItem* playlist = 
            dynamic_cast<const SpotifyPlaylistItem*>(obj);
        if (!playlist) continue;

        PlaylistItem playlistItem = {
            playlist->title(),
            playlist->imageUrl(),
            playlist->href(),
            playlist->id(),
        };
        m_items.append(QVariant::fromValue(playlistItem));
    }

    if (m_items.isEmpty()) return;

    beginInsertRows(QModelIndex(), 0, m_items.size()-1);
    endInsertRows();
}

QVariantMap SpotifyUserPlaylistsListModel::get(int i) const
{
    QModelIndex index = this->index(i, 0);
    QHash<int, QByteArray> roles = roleNames();
    QVariantMap map;
    for (QHash<int, QByteArray>::const_iterator cit = roles.cbegin();
        cit != roles.cend();
        cit++)
    {
        map[cit.value()] = index.data(cit.key());
    }
    return map;
}

void SpotifyUserPlaylistsListModel::coverImageReceived(const QString& id, const QUrl& imagePath) {
    if (m_items.isEmpty()) return;

    for (size_t i = 0; i < m_items.size(); i++) {

        if (!m_items.at(i).canConvert<PlaylistItem>() || !m_items[i].convert(QMetaType::fromType<PlaylistItem>())) {
            continue;
        }

        PlaylistItem item = qvariant_cast<PlaylistItem>(m_items.at(i));
        if (item.id != id) continue;

        item.imageUrl = imagePath;
        m_items[i] = QVariant::fromValue(item);
        emit dataChanged(index(i), index(i), { IMAGE_URL });
        break;
    }
}

#include "spotify/SpotifyPlaylistListModel.h"
#include "SelectionModel.h"
#include "SpotifyPlaylistListModel.h"
#include "SpotifyReceivedPlaylistElement.h"

SpotifyPlaylistListModel::SpotifyPlaylistListModel(QObject* parent) :
    SelectionModel(parent),
    m_playlist(nullptr)
{}

SpotifyPlaylistListModel::~SpotifyPlaylistListModel()
{}

QVariant SpotifyPlaylistListModel::data(const QModelIndex& index, int role) const
{
    return SelectionModel::data(index, role);
}

QHash<int, QByteArray> SpotifyPlaylistListModel::roleNames() const
{
    QHash<int, QByteArray> roles = SelectionModel::roleNames();
    roles[NAME] = "name";
    roles[ARTISTS] = "artists";
    roles[ALBUM] = "albums";
    return roles;
}

void SpotifyPlaylistListModel::setPlaylist(SpotifyReceivedPlaylistElement* playlist)
{
    if (!playlist) return;

    if (m_playlist)
    {
        m_playlist->deleteLater();
        m_playlist = nullptr;
    }

    m_playlist = playlist;
    m_playlist->setParent(this);

    if (m_playlist->failed()) return;

    emit playlistLoaded();
    emit nameChanged();
    emit imageHrefChanged();
    emit idChanged();
}

QString SpotifyPlaylistListModel::name() const
{
    if (!m_playlist) return QString();

    return m_playlist->name();
}

QUrl SpotifyPlaylistListModel::imageHref() const
{
    if (!m_playlist) return QUrl();

    return m_playlist->imageHref();
}

QString SpotifyPlaylistListModel::id() const
{
    if (!m_playlist) return QString();

    return m_playlist->id();
}
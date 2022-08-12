#include "AlbumTrackListModel.h"

AlbumTracksListModel::AlbumTracksListModel() :
    QAbstractListModel(nullptr)
{}

AlbumTracksListModel::~AlbumTracksListModel()
{}

int AlbumTracksListModel::rowCount(const QModelIndex &parent) const
{
    return m_trackList.count();
}

QVariant AlbumTracksListModel::data(const QModelIndex& index, int role) const
{
    if (index.column() == 0 && index.row() >= 0 && index.row() < rowCount())
    {
        // Returning the name of the track name.
        if (role == TRACK_NAME)
        {
            return m_trackList.at(index.row()).trackName;
        }
        // Returning the artists list.
        else if (role == ARTISTS)
        {
            return m_trackList.at(index.row()).artistsName;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> AlbumTracksListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TRACK_NAME] = "trackName";
    roles[ARTISTS] = "artists";
    return roles;
}

void AlbumTracksListModel::retrieveTracksFromAlbumName()
{
    // Check if albumName is not empty.
    if (m_albumName.isEmpty())
        return;

    // Remove existing tracks in the list.
    if (rowCount() > 0)
    {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        m_trackList.clear();
        endRemoveRows();
    }

    // Get the list of tracks from an album.
    QList<MusicCollectionList::TrackInfo> trackList =
            MusicCollectionList::instance()->retrieveTrackListFromAlbumName(m_albumName);

    // Update the view with the new list.
    if (!trackList.isEmpty())
    {
        m_trackList = trackList;
        beginInsertRows(QModelIndex(), 0, rowCount()-1);
        endInsertRows();
    }
}

QString AlbumTracksListModel::album() const
{
    return m_albumName;
}

void AlbumTracksListModel::setAlbum(const QString& album)
{
    if (album != m_albumName)
    {
        // Update album name and notify the view.
        m_albumName = album;
        retrieveTracksFromAlbumName();
        emit albumUpdated();
    }
}

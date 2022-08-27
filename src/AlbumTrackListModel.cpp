#include "AlbumTrackListModel.h"

AlbumTracksListModel::AlbumTracksListModel(QObject* parent) :
    SelectionModel(parent)
{}

AlbumTracksListModel::~AlbumTracksListModel()
{}

QVariant AlbumTracksListModel::data(const QModelIndex& index, int role) const
{
    if (index.column() == 0 && index.row() >= 0 && index.row() < rowCount())
    {
        QVariant variant = item(index.row());
        // Returning the name of the track name.
        if (role == TRACK_NAME)
        {
            return qvariant_cast<MusicCollectionList::TrackInfo>(variant).trackName;
        }
        else if (role == TRACK_NUMBER)
        {
            return qvariant_cast<MusicCollectionList::TrackInfo>(variant).trackNumber;
        }
        // Returning the artists list.
        else if (role == ARTISTS)
        {
            return qvariant_cast<MusicCollectionList::TrackInfo>(variant).artistsName;
        }
        else if (role == FILE_PATH)
        {
            return qvariant_cast<MusicCollectionList::TrackInfo>(variant).filePath;
        }
    }

    return SelectionModel::data(index, role);
}

QHash<int, QByteArray> AlbumTracksListModel::roleNames() const
{
    QHash<int, QByteArray> roles = SelectionModel::roleNames();
    roles[TRACK_NAME] = "trackName";
    roles[TRACK_NUMBER] = "trackNumber";
    roles[ARTISTS] = "artists";
    roles[FILE_PATH] = "filePath";
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
        clear();
        endRemoveRows();
    }

    // Get the list of tracks from an album.
    QList<MusicCollectionList::TrackInfo> trackList =
            MusicCollectionList::instance()->retrieveTrackListFromAlbumName(m_albumName);

    // Update the view with the new list.
    if (!trackList.isEmpty())
    {
        // Storing the trackList into a list of variant.
        QVariantList variantList(trackList.size());
        for (int i = 0; i < variantList.size(); i++)
        {
            variantList[i] = QVariant::fromValue(trackList.at(i));
        }

        // Sending the variant list to the SelectionModel parent class.
        setItemList(variantList);

        // Notify the view.
        beginInsertRows(QModelIndex(), 0, rowCount()-1);
        endInsertRows();
    }
    else
    {
        // If no track found, send an empty list to the upper class.
        setItemList(QVariantList());
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

QStringList AlbumTracksListModel::getTracksList() const
{
    // Getting the list of all the tracks from the SelectionModel class.
    QVariantList variantList = itemList();

    // Retrieve the file path from the list and store it into String QList.
    QStringList tracksPathList(variantList.size());
    for (int i = 0; i < tracksPathList.size(); i++)
    {
        tracksPathList[i] =
                qvariant_cast<MusicCollectionList::TrackInfo>(
                    variantList[i]).filePath;
    }

    // Returning the list.
    return tracksPathList;
}

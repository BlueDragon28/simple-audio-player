#include "PlaylistModel.h"
#include "SelectionModel.h"
#include <qabstractitemmodel.h>
#include <qhash.h>

PlaylistModel::PlaylistModel(QObject* parent) :
    SelectionModel(parent)
{
    QVariantList fakeTracksList = QVariantList();

    Track track1 = {
        "dummyFilePath",
        "Track 1",
        "Artist 1, Artist 2"
    };
    fakeTracksList.append(QVariant::fromValue(track1));

    track1.name = "Track 2";
    fakeTracksList.append(QVariant::fromValue(track1));

    track1.name = "Track 3";
    fakeTracksList.append(QVariant::fromValue(track1));

    track1.name = "Track 4";
    fakeTracksList.append(QVariant::fromValue(track1));

    setItemList(fakeTracksList);
}

PlaylistModel::~PlaylistModel()
{}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
    if (index.column() == 0 && index.row() >= 0 && index.row() < rowCount()) 
    {
        Track trackInfo = qvariant_cast<Track>(item(index.row()));

        switch (role) 
        {
        case FILEPATH: 
        {
            return trackInfo.filepath;
        } break;

        case NAME:
        {
            return trackInfo.name;
        } break;

        case ARTISTS:
        {
            return trackInfo.artists;
        } break;
        }
    }

    return SelectionModel::data(index, role);
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles = SelectionModel::roleNames();
    roles[FILEPATH] = "filepath";
    roles[NAME] = "name";
    roles[ARTISTS] = "artists";
    return roles;
}

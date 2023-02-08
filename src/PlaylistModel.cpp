#include "PlaylistModel.h"
#include "SelectionModel.h"
#include "TrackTag.h"
#include <qabstractitemmodel.h>
#include <qfileinfo.h>
#include <qurl.h>
#include <qhash.h>

PlaylistModel::PlaylistModel(QObject* parent) :
    SelectionModel(parent)
{}

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

void PlaylistModel::add(const QVariantList& vFilePath)
{
    if (vFilePath.isEmpty()) 
    {
        return;
    }

    QVariantList vTracksList;
    for (const QVariant& variant : vFilePath)
    {
        if (!variant.canConvert<QString>())
        {
            continue;
        }

        QString filePath = variant.toString();

        if (!QFileInfo::exists(filePath))
        {
            continue;
        }

        bool retrieveTagSuccess = false;
        TrackTag::Tag fileTag = TrackTag::getTagFromFile(filePath, &retrieveTagSuccess);

        if (!retrieveTagSuccess)
        {
            continue;
        }

        Track trackTag = {
            filePath,
            fileTag.title,
            fileTag.artist
        };

        vTracksList.append(QVariant::fromValue(trackTag));
    }

    if (!vTracksList.isEmpty())
    {
        const int lastVTracksListIndex = vTracksList.size() - 1;
        beginInsertRows(QModelIndex(), rowCount(), rowCount() + lastVTracksListIndex);
        addItemList(vTracksList);
        endInsertRows();
    }
}

QStringList PlaylistModel::pathList() const
{
    const QVariantList variantList = itemList();

    if (variantList.isEmpty())
    {
        return {};
    }

    QStringList paths;
    for (const QVariant& variant : variantList)
    {
        if (!variant.canConvert<Track>())
        {
            continue;
        }

        Track track = qvariant_cast<Track>(variant);

        paths.append(track.filepath);
    }

    return paths;
}

QStringList PlaylistModel::selectedTracksList() const
{
    QVariantList variantList = selectedItemList();

    QStringList filePathList;

    // Retrieving the file path from the list and store it into the file path string.
    foreach (const QVariant& variant, variantList)
    {
        Track trackInfo =
                qvariant_cast<Track>(variant);
        filePathList.append(trackInfo.filepath);
    }

    return filePathList;
}

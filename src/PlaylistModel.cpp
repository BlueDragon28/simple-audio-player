#include "PlaylistModel.h"
#include "SelectionModel.h"
#include "TrackTag.h"
#include <qabstractitemmodel.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qurl.h>
#include <qhash.h>
#include <qsavefile.h>

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

void PlaylistModel::saveToJSON(const QString& jsonPath) const
{
    QByteArray playlistJson = prepareJSON();
    bool result = saveToFile(jsonPath, playlistJson);
}

QByteArray PlaylistModel::prepareJSON() const
{
    QJsonDocument jsonDocument;
    jsonDocument.setObject(setJSONRootObject());
    return jsonDocument.toJson(QJsonDocument::Indented);
}

QJsonObject PlaylistModel::setJSONRootObject() const
{
    QJsonObject rootObject;
    rootObject.insert("data", jsonArrayOfPlaylistTracks());
    return rootObject;
}

QJsonArray PlaylistModel::jsonArrayOfPlaylistTracks() const
{
    QVariantList vItemList = itemList();

    QJsonArray jsonTracksList;

    for (const QVariant& vTrack : vItemList)
    {
        if (!vTrack.isValid() || !vTrack.canConvert<Track>())
        {
            continue;
        }

        Track track = qvariant_cast<Track>(vTrack);

        QJsonObject jsonTrack;
        jsonTrack.insert("filepath", track.filepath);
        jsonTrack.insert("title", track.name);
        jsonTrack.insert("artists", track.artists);
        
        jsonTracksList.append(jsonTrack);
    }

    return jsonTracksList;
}

bool PlaylistModel::saveToFile(const QString& filePath, const QByteArray& jsonDocument) const
{
    QSaveFile file(filePath);

    if (!file.open(QSaveFile::WriteOnly | QSaveFile::Text))
    {
        return false;
    }
    
    QTextStream out(&file);
    out << jsonDocument;

    return file.commit();
}

void PlaylistModel::loadFromJSON(const QString& jsonPath) 
{
    bool isOpenSuccess = false;
    QByteArray jsonStrData = readFromFile(jsonPath, &isOpenSuccess);

    if (!isOpenSuccess)
    {
        return;
    }

    parseJSON(jsonStrData);
}

QByteArray PlaylistModel::readFromFile(const QString& filePath, bool* result) const
{
    QFile file(filePath);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        if (result)
        {
            *result = false;
        }
        return QByteArray();
    }

    QTextStream stream(&file);
    QString fileData = stream.readAll();

    *result = true;
    return fileData.toUtf8();
}

void PlaylistModel::parseJSON(const QByteArray& jsonData)
{
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "anErrorOccurred";
        return;
    }

    if (!extractRootObject(jsonDocument))
    {
        qDebug() << "Failed to parse QJsonDocument";
    }
}

bool PlaylistModel::extractRootObject(const QJsonDocument& jsonDocument)
{
    if (jsonDocument.isEmpty() || !jsonDocument.isObject())
    {
        qDebug() << "JSON Document is or is not an object";
        return false;
    }

    QJsonObject rootObject = jsonDocument.object();
    
    if (rootObject.isEmpty())
    {
        qDebug() << "Root object is empty";
        return false;
    }

    QJsonValue dataValue = rootObject.value("data");

    return extractPlaylistList(dataValue);
}

bool PlaylistModel::extractTrackFromQJsonValue(
        const QJsonValue& item, 
        QList<Track>& tracksList) const 
{
    if (item.isUndefined() || item.isNull() || !item.isObject())
    {
        qDebug() << "Array item is undefined, null or is not an object";
        return false;
    }

    QJsonObject trackObject = item.toObject();

    if (trackObject.isEmpty() ||
        !trackObject.contains("artists") ||
        !trackObject.contains("filepath") ||
        !trackObject.contains("title"))
    {
        qDebug() << "Invalid track object";
        return false;
    }

    QVariant vFilePath = trackObject.value("filepath");
    QVariant vTitle = trackObject.value("title");
    QVariant vArtists = trackObject.value("artists");

    if (!vFilePath.isValid() || !vFilePath.convert(QMetaType::fromType<QString>()) ||
        !vTitle.isValid() || !vTitle.convert(QMetaType::fromType<QString>()) ||
        !vArtists.isValid() || !vArtists.convert(QMetaType::fromType<QString>()))
    {
        qDebug() << "Invalid tracks QVariant item";
        return false;
    }

    Track track = {
        vFilePath.toString(),
        vTitle.toString(),
        vArtists.toString()
    };

    tracksList.append(track);
    return true;
}

bool PlaylistModel::extractPlaylistList(const QJsonValue& playlistValue)
{
    if (playlistValue.isUndefined() || playlistValue.isNull() || !playlistValue.isArray())
    {
        qDebug() << "Playlist list is undefined, null or is not an array";
        return false;
    }

    QJsonArray playlistArray = playlistValue.toArray();
    
    QList<Track> tracksList(playlistArray.size());

    for (const QJsonValue& item : playlistArray)
    {
        if (!extractTrackFromQJsonValue(item, tracksList))
        {
            qDebug() << "Failed to extrack a track from a QJsonValue";
            return false;
        }
    }

    applyPlaylist(tracksList);
    return true;
}

void PlaylistModel::applyPlaylist(const QList<Track>& tracksList)
{
    for (const Track& track : tracksList)
    {
        qDebug() << "filePath:" << track.filepath << "title:" << track.name << "artists:" << track.artists;
    }
}

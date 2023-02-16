#ifndef SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_
#define SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_

#include "SelectionModel.h"
#include <qhash.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlist.h>
#include <qnamespace.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PlaylistModel : public SelectionModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QStringList pathList READ pathList);

    enum TracksRole {
        FILEPATH = MAX_ROLE,
        NAME,
        ARTISTS
    };

public slots:
    /*
    Add files to the list.
    It retrieve the tags from the file.
    */
    void add(const QVariantList& vFilePath);

    QStringList selectedTracksList() const;

    /*
    Save the playlist to a file.
    */
    void saveToJSON(const QString& jsonPath) const;
    
    /*
    Load a playlist from a JSON file.
    */
    void loadFromJSON(const QString& jsonPath);

public:
    PlaylistModel(QObject* parent = nullptr);
    virtual ~PlaylistModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    struct Track {
        QString filepath;
        QString name;
        QString artists;
    };

    QStringList pathList() const;

    QByteArray prepareJSON() const;
    QJsonObject setJSONRootObject() const;
    QJsonArray jsonArrayOfPlaylistTracks() const;
    bool saveToFile(const QString& filePath, const QByteArray& jsonDocument) const;

    QByteArray readFromFile(const QString& filePath, bool* result = nullptr) const;
    void parseJSON(const QByteArray& jsonData);
    bool extractRootObject(const QJsonDocument& jsonDocument);
    bool extractTrackFromQJsonValue(const QJsonValue& item, QList<Track>& tracksList) const;
    bool extractPlaylistList(const QJsonValue& playlistValue);
    void applyPlaylist(const QList<Track>& tracksList);
};

#endif // SIMPLEAUDIOPLAYER_PLAYLISTMODEL_H_

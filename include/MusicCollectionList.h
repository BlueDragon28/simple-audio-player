#ifndef SIMPLEAUDIOPLAYER_MUSICCOLLECTIONLIST_H
#define SIMPLEAUDIOPLAYER_MUSICCOLLECTIONLIST_H

#include "TrackTag.h"
#include <QObject>
#include <QtQml/qqmlcomponent.h>
#include <QString>
#include <QSqlDatabase>
#include <QFileInfo>

#include <mutex>
#include <thread>
#include <memory>
#include <atomic>

#define TRACKS_NAME "Tracks"
#define ALBUMS_NAME "Albums"
#define ARTISTS_NAME "Artists"

/*
This class is used to store the user music collection.
Based on folders path, the file will list all the music available.
*/
class MusicCollectionList : public QObject
{
    Q_OBJECT

    MusicCollectionList();

public:
    struct TrackInfo
    {
        QString filePath;
        QString trackName;
        QString albumName;
        QString artistsName;
    };

    ~MusicCollectionList();

    // Static method to retrieve and instanciate the class.
    static MusicCollectionList* instance();

    // Does the music collection has been read.
    bool isCollectionRead() const;

    // Number of albums in the collection.
    int numberOfItemInTable(const QString& tableName) const;

    // Retrieve names from table (tableName).
    QStringList retrieveNamesFromTable(const QString& tableName) const;

    // Retrieve the first found tracks file path of an album name.
    QString retrieveFilePathFromAlbumName(const QString& albumName) const;

    // Retrieve the track list from an album name.
    QList<TrackInfo> retrieveTrackListFromAlbumName(const QString& albumName) const;

signals:
    void listUpdated(); // Signal called when the thread (m_parsingThread) stop processing.

private:
    // Creating the SQL tables.
    void createTables();

    // Parsing the music collection.
    void parseCollection();

    // Getting the list of all the files in the music collection folders recursively.
    QFileInfoList getFilesInfo() const;

    // Retrieve the tags of all the files available in the music collection folders.
    void retrieveTagFromFiles(const QFileInfoList& filesInfo);

    // Insert a tag in the SQL table.
    void insertTag(const QFileInfo& fileInfo, TrackTag::Tag& tag);

    // Insert name into table (tableName).
    long long insertNameIntoTable(const QString& name, const QString& tableName);

    // Find name from table (tableName).
    long long findNameFromTable(const QString& name, const QString& tableName);

    bool m_error;

    // A SQL database to store the music list.
    QSqlDatabase m_db;
    std::mutex m_dbMutex;

    // List of strings holding the folders of the music collection.
    QStringList m_foldersList;

    // The parsing of the music collection is done in another thread.
    std::thread m_parsingThread;

    // Static instance of the class.
    static std::unique_ptr<MusicCollectionList> _instance;

    // Does the music collection has been read.
    std::atomic<bool> m_isCollectionRead;
};

#endif // SIMPLEAUDIOPLAYER_MUSICCOLLECTIONLIST_H

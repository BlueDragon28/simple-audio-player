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

/*
This class is used to store the user music collection.
Based on folders path, the file will list all the music available.
*/
class MusicCollectionList : public QObject
{
    Q_OBJECT

    MusicCollectionList();

public:
    ~MusicCollectionList();

    // Static method to retrieve and instanciate the class.
    static MusicCollectionList* instance();

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
};

#endif // SIMPLEAUDIOPLAYER_MUSICCOLLECTIONLIST_H

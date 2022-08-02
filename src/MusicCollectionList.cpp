#include "MusicCollectionList.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDirIterator>

std::unique_ptr<MusicCollectionList> MusicCollectionList::_instance = nullptr;

#define TRACKS_NAME "Tracks"
#define ALBUMS_NAME "Albums"
#define ARTISTS_NAME "Artists"

// MACRO to help create SQL tables and check if the creation successed.
#define SQL_CREATE_TABLE(statement, tableName, sqlQuery) \
    if (!query.exec(statement)) \
    { \
        qDebug() << QString("Failed to create SQL table " tableName " ") + query.lastError().text(); \
        m_error = true; \
        query.clear(); \
        return; \
    } \
    query.clear();

#define CHECK_ERROR(valueToReturn) \
    if (m_error) \
    { return valueToReturn; }

MusicCollectionList::MusicCollectionList() :
    m_error(false),
    m_db(QSqlDatabase::addDatabase("QSQLITE")),
    m_foldersList(QStandardPaths::standardLocations(QStandardPaths::MusicLocation)) // Returning the a list of the user music locations.
{
    // Creating a SQL database in memory.
    m_db.setDatabaseName(":memory:");
    if (!m_db.open())
    {
        m_error = true;
        return;
    }

    createTables();

    // Parsing the user music folders collection in another thread.
    m_parsingThread = std::thread(&MusicCollectionList::parseCollection, this);
}

MusicCollectionList::~MusicCollectionList()
{
    // Waiting for the parsing thread to stop before leaving.
    if (m_parsingThread.joinable())
    {
        m_parsingThread.join();
    }
}

MusicCollectionList* MusicCollectionList::instance()
{
    // If no instance exists, create it.
    if (!_instance)
    {
        _instance = std::unique_ptr<MusicCollectionList>(new MusicCollectionList());
    }

    // Return the instance.
    return _instance.get();
}

void MusicCollectionList::createTables()
{
    if (m_error)
        return;

    QSqlQuery query(m_db);

    // Creating the TracksName SQL table, it hold all the tacks.
    QString statement =
            "CREATE TABLE " TRACKS_NAME " ("
            "   ID INTEGER PRIMARY KEY,"
            "   filePath TEXT,"
            "   trackName TEXT,"
            "   album INTEGER,"
            "   artists INTEGER);";

    SQL_CREATE_TABLE(statement, TRACKS_NAME, query);

    // Creating the Albums SQL table.
    statement =
            "CREATE TABLE " ALBUMS_NAME " ("
            "   ID INTEGER PRIMARY KEY,"
            "   name TEXT);";

    SQL_CREATE_TABLE(statement, ALBUMS_NAME, query);

    // Creating the Artists SQL table.
    statement =
            "CREATE TABLE " ARTISTS_NAME " ("
            "   ID INTEGER PRIMARY KEY,"
            "   name TEXT);";

    SQL_CREATE_TABLE(statement, ARTISTS_NAME, query);
}

void MusicCollectionList::parseCollection()
{
    // Check if there is an error. If so then stop the thread.
    CHECK_ERROR();

    // Getting the list of all the files in the music collection folders.
    QFileInfoList filesInfo = getFilesInfo();

    CHECK_ERROR();

    // Getting the tags from the files.
    retrieveTagFromFiles(filesInfo);

    emit listUpdated();
}

QFileInfoList MusicCollectionList::getFilesInfo() const
{
    QFileInfoList filesInfo;

    // Iterate recursively into the music collection folders to get all the files.
    foreach (const QString& folder, m_foldersList)
    {
        QDirIterator dirIt(folder, QDir::Files, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
        while (dirIt.hasNext())
        {
            QFileInfo fileInfo = dirIt.nextFileInfo();
            if (fileInfo.exists())
                filesInfo.append(fileInfo);

            // Check if there is an error in the music collection, Is so then stop iterating.
            CHECK_ERROR(QFileInfoList());
        }
    }

    return filesInfo;
}

void MusicCollectionList::retrieveTagFromFiles(const QFileInfoList& filesInfo)
{
    // For each file in the list, getting the tag.
    foreach(const QFileInfo& fileInfo, filesInfo)
    {
        // Getting the tag of the file.
        bool success = false;
        TrackTag::Tag tag = TrackTag::getTagFromFile(fileInfo.absoluteFilePath(), &success);

        // If success, store the tag in the SQL table.
        if (success)
        {
            insertTag(fileInfo, tag);

            // Check if there is an error in the music collection, Is so then stop iterating.
            CHECK_ERROR();
        }
    }
}

void MusicCollectionList::insertTag(const QFileInfo& fileInfo, TrackTag::Tag& tag)
{
    // At least the title must be available.
    if (tag.title.isEmpty() || !fileInfo.isReadable())
    {
        return;
    }

    // Replace other field with unknown if empty.
    if (tag.album.isEmpty())
    {
        tag.album = "Unknown";
    }
    if (tag.artist.isEmpty())
    {
        tag.artist = "Unknown";
    }

    // Insert albums name.
    long long albumID = insertNameIntoTable(tag.album, ALBUMS_NAME);

    if (albumID < 0)
    {
        return;
    }

    // Insert artists name.
    long long artistID = insertNameIntoTable(tag.artist, ARTISTS_NAME);

    if (artistID < 0)
    {
        return;
    }

    // Insert tracks.
    QString statement = QString(
                "INSERT INTO " TRACKS_NAME " (filePath, trackName, album, artists) "
                "VALUES (\"%1\", \"%2\", %3, %4);")
            .arg(fileInfo.absoluteFilePath(), tag.title.replace("'", "''").replace("\"", "\"\""))
                    .arg(albumID).arg(artistID);

    QSqlQuery query(m_db);
    if (!query.exec(statement))
    {
        qDebug() << QString("Failed to insert tacks %1 into table %2. %3").arg(tag.title, TRACKS_NAME, query.lastError().text());
    }
}

long long MusicCollectionList::insertNameIntoTable(const QString& name, const QString& tableName)
{
    if (name.isEmpty())
    {
        return -1;
    }

    // Find if name already exist.
    long long nameID = findNameFromTable(name, tableName);
    if (nameID >= 0)
    {
        return nameID;
    }

    // If there is no name in the table, insert it.
    QString statement =
            "INSERT INTO " + tableName + " (name) "
            "VALUES (\"" + name + "\");";

    QSqlQuery query(m_db);
    if (!query.exec(statement))
    {
        qDebug() << QString("Failed to insert %1 into table %2. %3").arg(name, tableName, query.lastError().text());
        return -1;
    }

    // Then, find the ID of the newly inserted item.
    nameID = findNameFromTable(name, tableName);

    if (nameID >= 0)
    {
        return nameID;
    }
    else
    {
        return -1;
    }
}

long long MusicCollectionList::findNameFromTable(const QString& name, const QString& tableName)
{
    // Returning the ID of the element with the name (name) inside the table (tableName).
    if (name.isEmpty())
    {
        return -1;
    }

    // Find if name already exist.
    QString statement =
            "SELECT "
            "   ID, "
            "   name "
            "FROM "
            "   " + tableName + " "
            "WHERE "
            "   name LIKE \"" + name + "\";";

    QSqlQuery query(m_db);
    if (query.exec(statement))
    {
        if (query.next())
        {
            bool result = false;
            long long albumID = query.value(0).toLongLong(&result);

            if (result)
            {
                return albumID;
            }
        }
    }
    else
    {
        qDebug() << QString("Failed to query name from table %1. %2").arg(tableName, query.lastError().text());
    }

    return -1;
}

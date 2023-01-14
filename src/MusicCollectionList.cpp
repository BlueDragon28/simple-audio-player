#include "MusicCollectionList.h"
#include "AppConfig.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDirIterator>
#include <qforeach.h>
#include <qnamespace.h>
#include <qsqlquery.h>

std::unique_ptr<MusicCollectionList> MusicCollectionList::_instance = nullptr;

// MACRO to help create SQL tables and check if the creation successed.
#define SQL_CREATE_TABLE(statement, tableName, sqlQuery) \
    if (!sqlQuery.exec(statement)) \
    { \
        qDebug() << QString("Failed to create SQL table " tableName " ") + sqlQuery.lastError().text(); \
        m_error = true; \
        sqlQuery.clear(); \
        return; \
    } \
    sqlQuery.clear();

#define CHECK_ERROR(valueToReturn) \
    if (m_error) \
    { return valueToReturn; }

MusicCollectionList::MusicCollectionList() :
    m_error(false),
    m_db(QSqlDatabase::addDatabase("QSQLITE")),
    m_foldersList(AppConfig::getMusicCollectionPathList()), // Returning the a list of the user music locations.
    m_isCollectionRead(false)
{
    // Set the music collections folders list.
    // If there is a list available, use it.
    bool exists = false;
    QStringList folderList = AppConfig::getMusicCollectionPathList(&exists);
    if (exists)
    {
        m_foldersList = folderList;
    }
    // Otherwise, use the default music folders entry of the system.
    else
    {
        m_foldersList = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    }

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
            "   trackNumber INTEGER,"
            "   album INTEGER);";

    SQL_CREATE_TABLE(statement, TRACKS_NAME, query);

    // Creating the Albums SQL table.
    statement =
            "CREATE TABLE " ALBUMS_NAME " ("
            "   ID INTEGER PRIMARY KEY,"
            "   name TEXT);";

    SQL_CREATE_TABLE(statement, ALBUMS_NAME, query);

    // Creating the Artists Collection SQL table.
    statement =
            "CREATE TABLE " ARTISTS_COLLECTION_NAME " ("
            "   track INTEGER,"
            "   artist INTEGER);";

    SQL_CREATE_TABLE(statement, ARTISTS_COLLECTION_NAME, query);

    // Creating the Artiss SQL table.
    statement =
        "CREATE TABLE " ARTISTS_NAME "("
        "   ID INTEGER PRIMARY KEY,"
        "   name TEXT);";
    
    SQL_CREATE_TABLE(statement, ARTISTS_NAME, query);
}

void MusicCollectionList::parseCollection()
{
    std::lock_guard lock(m_dbMutex);

    // Check if there is an error. If so then stop the thread.
    CHECK_ERROR();

    // Getting the list of all the files in the music collection folders.
    QFileInfoList filesInfo = getFilesInfo();

    CHECK_ERROR();

    // Getting the tags from the files.
    retrieveTagFromFiles(filesInfo);

    m_isCollectionRead = true;
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

    // Insert tracks.
    long long trackID = insertTrack(fileInfo.absoluteFilePath(), tag.title, tag.trackNumber, albumID);

    if (trackID < 0)
    {
        return;
    }

    // Inserting artists into table artists and artistsCollection
    parseAndInsertArtists(trackID, tag.artist);
}

void MusicCollectionList::parseAndInsertArtists(long long trackID, const QString& artists)
{
    /*
    Split artists string by comma and inserting each one into tracks table and tracks collection table
    */
    if (trackID < 0 || artists.trimmed().isEmpty())
    {
        return;
    }

    const QStringList splittedArtists = artists.split(",", Qt::SkipEmptyParts);

    foreach (const QString& artist, splittedArtists)
    {
        // Insert artists name.
        long long artistID = insertArtistName(artist);

        if (artistID <= 0)
        {
            qDebug() << QString("Failed to insert artist %1 into table %2").arg(artist, ARTISTS_NAME);
            continue;
        }

        if (!insertArtistIntoCollection(trackID, artistID))
        {
            qDebug() << QString("Failed to insert artist %1 into table %2").arg(artist, ARTISTS_COLLECTION_NAME);
        }
    }
}

long long MusicCollectionList::insertArtistName(const QString& artistName)
{
    const QString artist = artistName.trimmed();

    if (artist.isEmpty())
    {
        return 0;
    }

    return insertNameIntoTable(artist, ARTISTS_NAME);
}

bool MusicCollectionList::insertArtistIntoCollection(long long trackID, long long artistID)
{
    if (trackID <= 0 || artistID <= 0)
    {
        return false;
    }

    QSqlQuery query(m_db);

    QString statement = QString(
        "INSERT INTO " ARTISTS_COLLECTION_NAME " (track, artist) "
        "VALUES (%1, %2);")
            .arg(trackID).arg(artistID);

    if (!query.exec(statement))
    {
        return false;
    }

    return true;
}

long long MusicCollectionList::insertTrack(const QString& filePath, const QString& trackName, int trackNumber, long long albumID)
{
    /*
    Inserting a track and retrieving the ID.
    */

    if (filePath.isEmpty() || trackName.trimmed().isEmpty() || albumID <= 0)
    {
        return -1;
    }

    // Insert tracks.
    QString statement = QString(
                "INSERT INTO " TRACKS_NAME " (filePath, trackName, trackNumber, album) "
                "VALUES (\"%1\", \"%2\", %3, %4);")
            .arg(QString(filePath).replace("\"", "\"\""), 
                trackName.trimmed().replace("\"", "\"\""))
            .arg(trackNumber).arg(albumID);

    QSqlQuery query(m_db);
    if (!query.exec(statement))
    {
        qDebug() << statement << QString("Failed to insert tracks %1 into table %2. %3").arg(trackName, TRACKS_NAME, query.lastError().text());
        return -1;
    }

    // Get track ID
    statement = QString(
        "SELECT ID FROM " TRACKS_NAME " WHERE filePath = \"%1\";")
            .arg(QString(filePath).replace("\"", "\"\""));

    long long trackID = -1;
    if (query.exec(statement)) 
    {
        if (query.next()) 
        {
            trackID = query.value(0).toLongLong();
        }
    }

    if (trackID < 0) 
    {
        qDebug() << statement << QString("Failed to get tracks ID of track %1 from table %2. %3").arg(trackName, TRACKS_NAME, query.lastError().text());
        return -1;
    }

    return trackID;
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
            "VALUES (\"" + QString(name).replace("\"", "\"\"") + "\");";

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
            "   name LIKE \"" + QString(name).replace("\"", "\"\"") + "\";";

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

int MusicCollectionList::numberOfItemInTable(const QString& tableName) const
{
    QSqlQuery query(m_db);

    // Retrieve the items in the SQL table.
    QString statement =
            QString("SELECT COUNT(%1) FROM %2;")
                .arg("name", tableName);

    // Count the numbers of items.
    int count = 0;
    if (query.exec(statement))
    {
        if (query.next())
        {
            count = query.value(0).toInt();
        }
    }

    // Return the number of items.
    return count;
}

QStringList MusicCollectionList::retrieveNamesFromTable(const QString &tableName) const
{
    QStringList namesList;

    // Retrieve the albums in the SQL table.
    QString statement =
            QString("SELECT %1 FROM %2 ORDER BY %1 ASC;")
                .arg("name", tableName);

    // Retrieve the names of the albums.
    QSqlQuery query(m_db);
    if (query.exec(statement))
    {
        while (query.next())
        {
            namesList.append(query.value(0).toString());
        }
    }

    // Return the names of the albums.
    return namesList;
}

QString MusicCollectionList::retrieveFilePathFromAlbumName(const QString& albumName) const
{
    // Retrieve a file path of a track from the album (albumName).
    QString statement = QString(
        "SELECT " TRACKS_NAME ".filePath "
        "FROM " TRACKS_NAME " "
        "INNER JOIN " ALBUMS_NAME " ON " ALBUMS_NAME ".ID = " TRACKS_NAME ".album "
        "WHERE name = \"%1\";").arg(QString(albumName).replace("\"", "\"\""));

    QSqlQuery query(m_db);
    if (query.exec(statement))
    {
        if (query.next())
        {
            return query.value(0).toString();
        }
    }
    else
    {
        qDebug() << QString("failed to query file path from album %1: %2").arg(albumName).arg(query.lastError().text());
    }

    return "";
}

QList<MusicCollectionList::TrackInfo> MusicCollectionList::retrieveTrackListFromAlbumName(const QString &albumName) const
{
    // Retrieve the files path of the tracks of an album (albumName).
    QString statement = QString(
        "SELECT filePath, trackName, trackNumber, " ALBUMS_NAME ".name AS albumName, GROUP_CONCAT(" ARTISTS_NAME ".name, \", \") AS artistsName "
        "FROM " TRACKS_NAME " "
        "INNER JOIN " ALBUMS_NAME " ON " ALBUMS_NAME ".ID = " TRACKS_NAME ".album "
        "INNER JOIN " ARTISTS_COLLECTION_NAME " ON " ARTISTS_COLLECTION_NAME ".track = " TRACKS_NAME ".ID "
        "INNER JOIN " ARTISTS_NAME " ON " ARTISTS_COLLECTION_NAME ".artist = " ARTISTS_NAME ".ID "
        "WHERE " ALBUMS_NAME ".name  = \"%1\" "
        "GROUP BY filePath "
        "ORDER BY trackNumber ASC;").arg(QString(albumName).replace("\"", "\"\""));

    QSqlQuery query(m_db);
    if (query.exec(statement))
    {
        // Store the retrieved information into a list.
        QList<TrackInfo> trackList;
        while (query.next())
        {
            TrackInfo track;
            track.filePath = query.value(0).toString();
            track.trackName = query.value(1).toString();
            track.trackNumber = query.value(2).toInt();
            track.albumName = query.value(3).toString();
            track.artistsName = query.value(4).toString();
            trackList.append(track);
        }

        // Returning the list.
        return trackList;
    }
    else
    {
        // If an error occured, print the statement and the error.
        qDebug() << statement << "failed to query album's tracks list: " + query.lastError().text();
    }

    return QList<TrackInfo>();
}

bool MusicCollectionList::isCollectionRead() const
{
    return m_isCollectionRead;
}

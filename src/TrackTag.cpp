#include "TrackTag.h"
#include "CoverArtTag.h"
#include <QFileInfo>
#include <QDir>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>

TrackTag::TrackTag()
{
    // Every time filePath is changed, reading the tag in another thread.
    connect(this, &TrackTag::filePathChanged, 
        [this]() {
            if (this->m_readTagThread.joinable())
                this->m_readTagThread.join();
            
            this->m_readTagThread = 
                std::thread(&TrackTag::readTag, this);
        });
}

TrackTag::~TrackTag()
{
    if (m_readTagThread.joinable())
        m_readTagThread.join();
    CoverArtTag::resetCoverImage();
}

/*
Set the file path to extract the tags.
*/
void TrackTag::setFilePath(const QString& filePath)
{
    if (filePath != m_filePath)
    {
        std::scoped_lock lock(m_filePathMutex);
        m_filePath = filePath;
        emit filePathChanged();
    }
}

/*
Return the current file path.
*/
QString TrackTag::filePath() const
{
    std::scoped_lock lock(m_filePathMutex);
    return m_filePath;
}

/*
Static method returning the tag of a file.
*/
TrackTag::Tag TrackTag::getTagFromFile(const QString& filePath)
{
    // If the filePath is empty or the file does not exists, return an empty tag list.
    if (filePath.isEmpty() || !QFileInfo::exists(filePath))
        return {};

    TrackTag::Tag tag;
    
    // Opening the file.
    TagLib::FileRef file(filePath.toLocal8Bit().constData());
    if (!file.isNull() && file.tag())
    {
        // Retrieve the tags.
        TagLib::Tag* t = file.tag();

        tag.title = QString::fromUtf8(t->title().toCString());
        tag.album = QString::fromUtf8(t->album().toCString());
        tag.artist = QString::fromUtf8(t->artist().toCString());
    }

    return tag;
}

/*
Inline method to update tag.
*/
template<typename T>
inline void TrackTag::updateTag(T& memberTagItem, T& newTagItem, void (TrackTag::*updateSignal)())
{
    bool tagUpdated = false;

    // Check if the new tag is different from the old tag. If it is, update it.
    if (newTagItem != memberTagItem)
    {
        memberTagItem = newTagItem;
        tagUpdated = true;
    }

    // If the tag is updated, notify it.
    if (tagUpdated)
    {
        emit (this->*updateSignal)();
    }
}

/*
Read the tag of the current file.
*/
void TrackTag::readTag()
{
    // Retrieve the tag from the file filePath.
    Tag tag = getTagFromFile(m_filePath);

    {
        std::scoped_lock lock(m_tagMutex);
        updateTag(m_tag.title, tag.title, &TrackTag::titleChanged);
        updateTag(m_tag.album, tag.album, &TrackTag::albumChanged);
        updateTag(m_tag.artist, tag.artist, &TrackTag::artistChanged);
    }

    // Get the cover art of the album.
    getCoverArt();
}

QString TrackTag::title() const
{
    std::scoped_lock lock(m_tagMutex);
    return m_tag.title;
}

QString TrackTag::album() const
{
    std::scoped_lock lock(m_tagMutex);
    return m_tag.album;
}

QString TrackTag::artist() const
{
    std::scoped_lock lock(m_tagMutex);
    return m_tag.artist;
}

void TrackTag::getCoverArt() 
{
    // Get the list of files inside the track directory.
    QDir trackDirectory = QFileInfo(m_filePath).absoluteDir();
    QFileInfoList filesInfo = trackDirectory.entryInfoList(QDir::Files);

    // Check if there is a file named "large_cover.png" or "small_cover.png" in the folder.
    QString filePath;
    foreach (const QFileInfo& fileInfo, filesInfo)
    {
        if (fileInfo.fileName() == "large_cover.png" ||
            fileInfo.fileName() == "small_cover.png")
        {
            filePath = fileInfo.absoluteFilePath();
            break;
        }
    }

    // If the cover is found, opening it.
    if (!filePath.isEmpty())
    {
        CoverArtTag::setCoverImage(QPixmap(filePath), m_tag.album);
        emit coverArtChanged();
    }
    // Otherwise, reset the cover.
    else
    {
        CoverArtTag::resetCoverImage();
        emit coverArtIsEmpty();
    }
}
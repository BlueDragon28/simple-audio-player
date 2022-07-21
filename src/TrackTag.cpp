#include "TrackTag.h"
#include <QFileInfo>
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
        tag.artist = QString::fromUtf8(t->album().toCString());
    }

    return tag;
}

/*
Read the tag of the current file.
*/
void TrackTag::readTag()
{
    std::scoped_lock lock(m_filePathMutex, m_tagMutex);

    // Retrieve the tag from the file filePath.
    Tag tag = getTagFromFile(m_filePath);

    // Update the tag.
    if (tag.title != m_tag.title)
    {
        m_tag.title = tag.title;
        emit titleChanged();
    }
    if (tag.album != m_tag.album)
    {
        m_tag.album = tag.album;
        emit albumChanged();
    }
    if (tag.artist != m_tag.artist)
    {
        m_tag.artist = tag.artist;
        emit artistChanged();
    }
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
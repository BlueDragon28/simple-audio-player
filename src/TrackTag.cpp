#include "TrackTag.h"
#include "CoverArtTag.h"
#include <QFileInfo>
#include <QDir>
#include <QBitmap>
#include <QImage>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/flacfile.h>
#include <taglib/flacpicture.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/mpegfile.h>

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
TrackTag::Tag TrackTag::getTagFromFile(const QString& filePath, bool* success)
{
    // If the filePath is empty or the file does not exists, return an empty tag list.
    if (filePath.isEmpty() || !QFileInfo::exists(filePath))
    {
        if (success)
        {
            *success = false;
        }

        return {};
    }

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

        if (success)
        {
            *success = true;
        }
    }
    else
    {
        if (success)
        {
            *success = false;
        }
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
    else
    {
        // Trying to get the cover image from the file.
        if (!m_filePath.isEmpty() && extractCoverArtFromFile())
        {
            emit coverArtChanged();
            return;
        }
        
        // Otherwise, reset the cover.
        CoverArtTag::resetCoverImage();
        emit coverArtIsEmpty();
    }
}

bool TrackTag::extractCoverArtFromFile()
{
    QString filePath = this->filePath();
    Tag tag = getTag();
    
    // Open as a FLAC file.
    bool result = extractFlacCoverArt(filePath, tag);

    if (!result)
    {
        result = extractMp3CoverArt(filePath, tag);
    }

    return result;
}

bool TrackTag::extractFlacCoverArt(const QString& filePath, const Tag& tag)
{
    bool result = false;

    // Open as a FLAC file.
    TagLib::FLAC::File flacFile = TagLib::FLAC::File(filePath.toLocal8Bit().constData());
    if (flacFile.isValid())
    {
        // Retrieve all the images inside the flac file.
        TagLib::List<TagLib::FLAC::Picture*> pictures = 
            flacFile.pictureList();
        
        if (!pictures.isEmpty())
        {
            // Get the first image and use it as the cover art.
            TagLib::FLAC::Picture* picture = pictures.front();

            // Open image from the data.
            QImage image = QImage::fromData(
                QByteArrayView(picture->data().data(), picture->data().size()));

            if (!image.isNull())
            {
                CoverArtTag::setCoverImage(QPixmap::fromImage(image), tag.album);
                result = true;
            }
        }
        
        // If no cover art image found, check if there are not on the ID3v2Tag.
        if (!result && flacFile.ID3v2Tag())
        {
            result = extractId3v2CoverArt(flacFile.ID3v2Tag(), tag);
        }
    }

    return result;
}

bool TrackTag::extractMp3CoverArt(const QString& filePath, const Tag& tag)
{
    bool result = false;

    // Open has a MPEG file.
    TagLib::MPEG::File mpegFile = TagLib::MPEG::File(filePath.toLocal8Bit().constData());
    if (mpegFile.isValid())
    {
        // Extract ID3v2 cover art if any.
        if (mpegFile.ID3v2Tag())
        {
            result = extractId3v2CoverArt(mpegFile.ID3v2Tag(), tag);
        }

        // Extract APE cover art if any.
        if (!result && mpegFile.APETag())
        {
            result = extractAPECoverArt(mpegFile.APETag(), tag);
        }
    }

    return result;
}

bool TrackTag::extractId3v2CoverArt(TagLib::ID3v2::Tag* tag, const Tag& albumTag)
{
    if (tag)
    {
        // Getting the cover list.
        const TagLib::ID3v2::FrameList& frameList = tag->frameList("APIC");
        if (!frameList.isEmpty())
        {
            const TagLib::ID3v2::AttachedPictureFrame* const cover = 
                (const TagLib::ID3v2::AttachedPictureFrame*)frameList.front();
            QImage image = QImage::fromData(
                QByteArrayView(cover->picture().data(), cover->picture().size()));
            
            if (!image.isNull())
            {
                CoverArtTag::setCoverImage(QPixmap::fromImage(image), albumTag.album);
                return true;
            }
        }
    }

    return false;
}

bool TrackTag::extractAPECoverArt(TagLib::APE::Tag* tag, const Tag& albumTag)
{
    if (tag)
    {
        const TagLib::APE::ItemListMap& list = tag->itemListMap();
        // Getting the cover art image.
        if (list.contains("COVER ART (FRONT)"))
        {
            TagLib::ByteVector item = list["COVER ART (FRONT)"].value();
            // Do not copy the file name of the file.
            int pos = item.find('\0');
            if (pos >= 0)
            {
                const TagLib::ByteVector& pic = item.mid(pos+1);

                // Opening the image.
                QImage image = QImage::fromData(
                    QByteArrayView(pic.data(), pic.size()));
                
                if (!image.isNull())
                {
                    CoverArtTag::setCoverImage(QPixmap::fromImage(image), albumTag.album);
                    return true;
                }
            }
        }
    }

    return false;
}

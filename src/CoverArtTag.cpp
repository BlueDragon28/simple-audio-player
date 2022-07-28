#include "CoverArtTag.h"

// Initialize the cover image.
QString CoverArtTag::m_albumName;
std::unique_ptr<QPixmap> CoverArtTag::m_coverImage;
std::mutex CoverArtTag::m_coverMutex;

void CoverArtTag::setCoverImage(const QPixmap& coverImage, const QString& albumName) 
{
    std::scoped_lock lock(m_coverMutex);
    m_coverImage = std::unique_ptr<QPixmap>(new QPixmap(coverImage));
    m_albumName = albumName;
}

QPixmap CoverArtTag::getCoverImage()
{
    std::scoped_lock lock(m_coverMutex);
    if (m_coverImage)
    {
        return *m_coverImage.get();
    }
    else
    {
        return QPixmap();
    }
}

QString CoverArtTag::getAlbumName()
{
    std::scoped_lock lock(m_coverMutex);
    return m_albumName;
}

void CoverArtTag::resetCoverImage()
{
    std::scoped_lock lock(m_coverMutex);
    m_coverImage.reset();
}
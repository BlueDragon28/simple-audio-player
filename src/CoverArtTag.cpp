#include "CoverArtTag.h"

// Initialize the cover image.
std::unique_ptr<QPixmap> CoverArtTag::m_coverImage;
std::mutex CoverArtTag::m_coverMutex;

void CoverArtTag::setCoverImage(const QPixmap& coverImage) 
{
    std::scoped_lock lock(m_coverMutex);
    m_coverImage = std::unique_ptr<QPixmap>(new QPixmap(coverImage));
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

void CoverArtTag::resetCoverImage()
{
    std::scoped_lock lock(m_coverMutex);
    m_coverImage.reset();
}
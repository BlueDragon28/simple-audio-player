#include "CoverArtTag.h"

// Initialize the cover image.
QPixmap CoverArtTag::m_coverImage;
std::mutex CoverArtTag::m_coverMutex;

void CoverArtTag::setCoverImage(const QPixmap& coverImage) 
{
    std::scoped_lock lock(m_coverMutex);
    m_coverImage = coverImage;
}

QPixmap CoverArtTag::getCoverImage()
{
    std::scoped_lock lock(m_coverMutex);
    return m_coverImage;
}

void CoverArtTag::resetCoverImage()
{
    std::scoped_lock lock(m_coverMutex);
    m_coverImage = QPixmap();
}
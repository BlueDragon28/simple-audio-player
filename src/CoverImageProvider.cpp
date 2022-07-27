#include <CoverImageProvider.h>
#include "CoverArtTag.h"

CoverImageProvider::CoverImageProvider() :
    QQuickImageProvider(QQuickImageProvider::Pixmap)
{}

QPixmap CoverImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    // Retrive the cover image.
    if (id == "cover")
    {
        QPixmap pixmap = CoverArtTag::getCoverImage();

        // Store in size the real size of the image.
        if (size)
        {
            *size = pixmap.size();
        }

        // If requested image is set, resize the image.
        if (requestedSize.width() > 0 && requestedSize.height() > 0)
        {
            pixmap = pixmap.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        return pixmap;
    }

    return QPixmap();
}
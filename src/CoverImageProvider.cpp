#include <CoverImageProvider.h>
#include <QFileInfo>
#include "CoverArtTag.h"
#include "MusicCollectionList.h"
#include "TrackTag.h"

CoverImageProvider::CoverImageProvider() :
    QQuickImageProvider(QQuickImageProvider::Pixmap)
{}

QPixmap CoverImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    QPixmap pixmap;

    // Get cover image or default image base on id.
    if (id == "empty")
    {
        pixmap = QPixmap(":/images/musiqueIcon.png");
    }
    else if (!id.isEmpty())
    {
        // If the request album is the same has the stored cover image, retrieving it.
        if (id == CoverArtTag::getAlbumName())
        {
            pixmap = CoverArtTag::getCoverImage();
        }
        // If not, then try to open the image using the file path stored in the album collection list.
        else
        {
            QString trackPath = MusicCollectionList::instance()->retrieveFilePathFromAlbumName(id);
            if (!trackPath.isEmpty())
            {
                pixmap = TrackTag::getCoverArt(trackPath);
            }
        }

        // If the pixmap is null, load the default image cover.
        if (pixmap.isNull())
        {
            pixmap = QPixmap(":/images/musiqueIcon.png");
        }
    }

    // Store in size the real size of the image.
    if (size)
    {
        *size = pixmap.size();
    }

    // If requested image is set, resize the image.
    if (requestedSize.width() > 0 && requestedSize.height() > 0 && !pixmap.isNull())
    {
        pixmap = pixmap.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    return pixmap;
}

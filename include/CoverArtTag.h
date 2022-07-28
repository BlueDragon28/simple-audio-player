#ifndef SIMPLEAUDIOPLAYER_COVERARTTAG_H_
#define SIMPLEAUDIOPLAYER_COVERARTTAG_H_

#include <QString>
#include <QPixmap>
#include <mutex>
#include <memory>

/*
This static class store the cover art of the current played image.
*/
class CoverArtTag
{
    /* 
    Delete constructor and destructor.
    */
    CoverArtTag() = delete;
    CoverArtTag(const CoverArtTag&) = delete;

public:
    /*
    Set, get and reset the cover image and album name.
    */
    static void setCoverImage(const QPixmap& coverImage, const QString& albumName);
    static QPixmap getCoverImage();
    static QString getAlbumName();
    static void resetCoverImage();

private:
    static QString m_albumName;
    static std::unique_ptr<QPixmap> m_coverImage;
    static std::mutex m_coverMutex;
};

#endif // SIMPLEAUDIOPLAYER_COVERARTTAG_H_
#ifndef SIMPLEAUDIOPLAYER_COVERARTTAG_H_
#define SIMPLEAUDIOPLAYER_COVERARTTAG_H_

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
    Set, get and reset the cover image.
    */
    static void setCoverImage(const QPixmap& coverImage);
    static QPixmap getCoverImage();
    static void resetCoverImage();

private:
    static std::unique_ptr<QPixmap> m_coverImage;
    static std::mutex m_coverMutex;
};

#endif // SIMPLEAUDIOPLAYER_COVERARTTAG_H_
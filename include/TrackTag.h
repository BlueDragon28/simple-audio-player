#ifndef SIMPLEAUDIOPLAYER_TRACKTAG_H_
#define SIMPLEAUDIOPLAYER_TRACKTAG_H_

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <thread>
#include <mutex>
#include <QPixmap>
#include <taglib/id3v2tag.h>
#include <taglib/apetag.h>

class TrackTag : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString album READ album NOTIFY albumChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)

public:
    struct Tag
    {
        QString title;
        QString album;
        QString artist;
    };

    TrackTag();
    ~TrackTag();

    /*
    Static method returning the tag of a file.
    */
    Q_INVOKABLE static Tag getTagFromFile(const QString& filePath, bool* success = nullptr);

    /*
    Get the cover art of an album.
    */
    static QPixmap getCoverArt(const QString& filePath);

signals:
    void filePathChanged();
    void titleChanged();
    void albumChanged();
    void artistChanged();
    void coverArtChanged();
    void coverArtIsEmpty();

private:
    /*
    Enumeration file type.
    */
    enum class AudioFileType
    {
        UNKNOWN,
        FLAC,
        ID3,
    };


    /*
    Set the file path to extract the tags.
    */
    void setFilePath(const QString& filePath);

    /*
    Return the current file path.
    */
    QString filePath() const;

    /*
    Returning the tag.
    */
    
    QString title() const;
    QString album() const;
    QString artist() const;

    // Return the tag struct.
    inline Tag getTag() const;

    /*
    Get file type base from the identification in the header of the file.
    */
    static AudioFileType getFileTypeFromHeader(const QString& filePath);

    /*
    Extracting the cover image from an audio file.
    */
    static QPixmap extractCoverArtFromFile(const QString& filePath);

    static QPixmap extractFlacCoverArt(const QString& filePath);
    static QPixmap extractMp3CoverArt(const QString& filePath);

    static QPixmap extractId3v2CoverArt(TagLib::ID3v2::Tag* tag);

    /*
    Inline method to update tag.
    */
    template<typename T>
    inline void updateTag(T& memberTagItem, T& newTagItem, void (TrackTag::*updateSignal)());

    /*
    Read the tag of the current file.
    */
    void readTag();

    // Current file path.
    QString m_filePath;
    mutable std::mutex m_filePathMutex;

    // Storing the tags of the current file.
    Tag m_tag;
    mutable std::mutex m_tagMutex;

    // Reading the tag of the current file in a separate file.
    std::thread m_readTagThread;
};

inline TrackTag::Tag TrackTag::getTag() const
{
    std::scoped_lock lock(m_tagMutex);
    return m_tag;
}

#endif // SIMPLEAUDIOPLAYER_TRACKTAG_H_

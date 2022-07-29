#ifndef SIMPLEAUDIOPLAYER_TRACKTAG_H_
#define SIMPLEAUDIOPLAYER_TRACKTAG_H_

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <thread>
#include <mutex>
#include <QPixmap>
#include <taglib/id3v2tag.h>

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
    Q_INVOKABLE static Tag getTagFromFile(const QString& filePath);

signals:
    void filePathChanged();
    void titleChanged();
    void albumChanged();
    void artistChanged();
    void coverArtChanged();
    void coverArtIsEmpty();

private:
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

    /*
    Get the cover art of the album.
    */
    void getCoverArt();

    /*
    Extracting the cover image from the audio file.
    */
    bool extractCoverArtFromFile();

    bool extractFlacCoverArt();
    bool extractId3v2CoverArt(TagLib::ID3v2::Tag* tag);

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

#endif // SIMPLEAUDIOPLAYER_TRACKTAG_H_
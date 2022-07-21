#ifndef SIMPLEAUDIOPLAYER_TRACKTAG_H_
#define SIMPLEAUDIOPLAYER_TRACKTAG_H_

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <thread>
#include <mutex>

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

    /*
    Static method returning the tag of a file.
    */
    Q_INVOKABLE static Tag getTagFromFile(const QString& filePath);

signals:
    void filePathChanged();
    void titleChanged();
    void albumChanged();
    void artistChanged();

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
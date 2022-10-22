#ifndef SIMPLEAUDIOPLAYER_SAPMPRIS_H_
#define SIMPLEAUDIOPLAYER_SAPMPRIS_H_

#include <QObject>
#include <qtmetamacros.h>
#include <QDBusObjectPath>

class SAPMPris : public QObject
{
    Q_OBJECT

    // Root mpris properties
    Q_PROPERTY(bool CanQuit READ canQuit)
    Q_PROPERTY(bool CanRaise READ canRaise)
    Q_PROPERTY(bool HasTrackList READ hasTrackList)
    Q_PROPERTY(QString Identity READ identity)
    Q_PROPERTY(QStringList SupportedUriSchemes READ supportedUriSchemes)
    Q_PROPERTY(QStringList SupportedMimeTypes READ supportedMimeTypes)

    // Player mpris properties
    Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
    Q_PROPERTY(double Rate READ rate WRITE setRate)
    Q_PROPERTY(QVariantMap Metadata READ metadata)
    Q_PROPERTY(double Volume READ volume WRITE setVolume)
    Q_PROPERTY(long long Position READ position)
    Q_PROPERTY(double MinimumRate READ minimumRate)
    Q_PROPERTY(double MaximumRate READ maximumRate)
    Q_PROPERTY(bool CanGoNext READ canNext)
    Q_PROPERTY(bool CanGoPrevious READ canPrevious)
    Q_PROPERTY(bool CanPlay READ canPlay)
    Q_PROPERTY(bool CanPause READ canPause)
    Q_PROPERTY(bool CanSeek READ canSeek)
    Q_PROPERTY(bool CanControl READ canControl)


public:
    SAPMPris(QObject* parent = nullptr);
    ~SAPMPris();

    // Root mpris methods.
public slots:
    void Raise();
    void Quit();

    // Player mpris methods.
    void Next();
    void Previous();
    void Pause();
    void PlayPause();
    void Stop();
    void Play();
    void Seek(long long offset);
    void SetPosition(const QDBusObjectPath& id, long long position);
    void OpenUri(const QString& uri);

public:

    // Player properties.
    enum class PlaybackStatus
    {
        PLAYING,
        PAUSED,
        STOPPED
    };

    struct MetaData
    {
        QString trackID;
        QString title;
        QString album;
        QString artists;

        QVariantMap toVariantMap() const;
    };

private:
    // Root properties.
    bool canQuit() const;
    bool canRaise() const;
    bool hasTrackList() const;
    QString identity() const;
    QStringList supportedUriSchemes() const;
    QStringList supportedMimeTypes() const;

    // Player properties.
    QString playbackStatus() const;
    void setPlaybackStatus(PlaybackStatus status);

    double rate() const;
    void setRate(double rate);

    QVariantMap metadata() const;
    void setMetadata(const MetaData& data);

    double volume() const;
    void setVolume(double volume);

    long long position() const;
    void setPosition(long long position);

    double minimumRate() const;
    double maximumRate() const;

    bool canNext() const;
    bool canPrevious() const;
    bool canPlay() const;
    bool canPause() const;
    bool canSeek() const;
    bool canControl() const;


    // Helper methods.
    static QString playbackStatusEnumToString(PlaybackStatus status);

    /*
    Enum with the type of data that can be send to MPRIS.
    */
    enum class NotifyType
    {
        UNKNOWN = -1,
        PLAYBACK_STATUS,
        METADATA
    };
    static QString notifyTypeToString(NotifyType type);

    /*
    Send a notification to DBus MPRIS interface.
    */
    void notify(NotifyType type, const QVariant& value);
    void notify(NotifyType type);

    /*
    Retrieve the data based on the type value.
    */
    QVariant getNotifyValue(NotifyType type);

    // Make the class SystemMediaControl a friend class
    friend class SystemMediaControl;

    // Members variables.
    QString m_playbackStatus;
    QVariantMap m_metadata;
    long long m_position;

    // Static variables
    static const char* _serviceName;
    static const char* _mprisObjectPath;
    static const char* _freeDesktopPath;
    static const char* _propertyChangedStr;
    static const char* _mprisPlayerEntity;
};

#endif // SIMPLEAUDIOPLAYER_SAPMPRIS_H_
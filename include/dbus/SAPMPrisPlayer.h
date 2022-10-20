#ifndef SIMPLEAUDIOPLAYER_SAPMPRISPLAYER_H_
#define SIMPLEAUDIOPLAYER_SAPMPRISPLAYER_H_

#include <QObject>
#include <qdbusextratypes.h>
#include <qtmetamacros.h>
#include <QDBusObjectPath>
#include <taglib/flacmetadatablock.h>

class SAPMPrisPlayer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("DBus MPRIS Player interface", "org.mpris.MediaPlayer2.Player")

    Q_PROPERTY(QString PlaybackStatus READ playbackStatus NOTIFY playbackStatusChanged)
    Q_PROPERTY(double Rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(QVariantMap Metadata READ metadata NOTIFY metadataChanged)
    Q_PROPERTY(double Volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(long long Position READ position NOTIFY positionChanged)
    Q_PROPERTY(double MinimumRate READ minimumRate)
    Q_PROPERTY(double MaximumRate READ maximumRate)
    Q_PROPERTY(bool CanGoNext READ canNext NOTIFY canNextChanged)
    Q_PROPERTY(bool CanGoPrevious READ canPrevious NOTIFY canPreviousChanged)
    Q_PROPERTY(bool CanPlay READ canPlay NOTIFY canPlayChanged)
    Q_PROPERTY(bool CanPause READ canPause NOTIFY canPauseChanged)
    Q_PROPERTY(bool CanSeek READ canSeek NOTIFY canSeekChanged)
    Q_PROPERTY(bool CanControl READ canControl NOTIFY canControlChanged)

public:
    SAPMPrisPlayer();

signals:
    void playbackStatusChanged();
    void rateChanged();
    void metadataChanged();
    void volumeChanged();
    void positionChanged();
    void canNextChanged();
    void canPreviousChanged();
    void canPlayChanged();
    void canPauseChanged();
    void canSeekChanged();
    void canControlChanged();

public slots:
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
    enum class PlaybackStatus
    {
        PLAYING,
        PAUSED,
        STOPPED
    };

    struct MetaData
    {
        QString trackID;

        QVariantMap toVariantMap() const;
    };

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

private:
    static QString playbackStatusEnumToString(PlaybackStatus status);

    QString m_playbackStatus;
    QVariantMap m_metadata;
    long long m_position;
};

#endif // SIMPLEAUDIOPLAYER_SAPMPRISPLAYER_H_
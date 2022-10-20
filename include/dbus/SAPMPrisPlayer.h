#ifndef SIMPLEAUDIOPLAYER_SAPMPRISPLAYER_H_
#define SIMPLEAUDIOPLAYER_SAPMPRISPLAYER_H_

#include <QObject>
#include <qtmetamacros.h>
#include <QDBusObjectPath>

class SAPMPrisPlayer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("DBus MPRIS Player interface", "org.mpris.MediaPlayer2.Player")

    Q_PROPERTY(QString PlaybackStatus READ playbackStatus NOTIFY playbackStatusChanged)
    Q_PROPERTY(double Rate READ rate WRITE setRate NOTIFY readChanged)
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
    void readChanged();
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
    QString playbackStatus() const;

    double rate() const;
    void setRate(double rate);

    QVariantMap metadata() const;

    double volume() const;
    void setVolume(double volume);

    double position() const;

    double minimumRate() const;
    double maximumRate() const;

    bool canNext() const;
    bool canPrevious() const;
    bool canPlay() const;
    bool canPause() const;
    bool canSeek() const;
    bool canControl() const;
};

#endif // SIMPLEAUDIOPLAYER_SAPMPRISPLAYER_H_
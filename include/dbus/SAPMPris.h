#ifndef SIMPLEAUDIOPLAYER_SAPMPRIS_H_
#define SIMPLEAUDIOPLAYER_SAPMPRIS_H_

#include <QObject>
#include <qtmetamacros.h>

class SAPMPris : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("DBus MPRIS interface", "org.mpris.MediaPlayer2")

    Q_PROPERTY(bool CanQuit READ canQuit NOTIFY canQuitChanged)
    Q_PROPERTY(bool CanRaise READ canRaise NOTIFY canRaiseChanged)
    Q_PROPERTY(bool HasTrackList READ hasTrackList NOTIFY hasTrackListChanged)
    Q_PROPERTY(QString Identity READ identity NOTIFY identityChanged)
    Q_PROPERTY(QStringList SupportedUriSchemes READ supportedUriSchemes NOTIFY supportedUriSchemesChanged)
    Q_PROPERTY(QStringList SupportedMimeTypes READ supportedMimeTypes NOTIFY supportedMimeTypesChanged)

public:
    SAPMPris();
    ~SAPMPris();

signals:
    void canQuitChanged();
    void canRaiseChanged();
    void hasTrackListChanged();
    void identityChanged();
    void supportedUriSchemesChanged();
    void supportedMimeTypesChanged();

public slots:
    void Raise();
    void Quit();

private:
    bool canQuit() const;
    bool canRaise() const;
    bool hasTrackList() const;
    QString identity() const;
    QStringList supportedUriSchemes() const;
    QStringList supportedMimeTypes() const;
};

#endif // SIMPLEAUDIOPLAYER_SAPMPRIS_H_
#ifndef SIMPLEAUDIOPLAYER_SMTCINTERFACE_H_
#define SIMPLEAUDIOPLAYER_SMTCINTERFACE_H_

#include <QObject>
#include <SAP_SMTC.h>

/*
This class is an interface between SystemMediaControl and the System Media Transport Control protocol.
*/
class SMTCInterface : public QObject
{
    Q_OBJECT

public:
    SMTCInterface(QObject* parent = nullptr);
    ~SMTCInterface();

    /*
    Notify System Media Transport Control about the status change of the playback.
    */
    void setPlaybackStatus(SMTC::PlaybackStatus status);

    /*
    Sending to System Media Transport Control the information about the new track.
    */
    void setTrackInfo(const QString& title, const QString& artists);

    /*
    Tell the system if it can go to the next track.
    If there is no track to play after the current one, this allow
    the system to inform the user about it.
    */
    void setCanNext(bool value);

// This signal are called everytime a media key is clicked.
signals:
    void playPause();
    void previous();
    void next();

private:
    SMTC::SAP_SMTC m_smtc;
};

#endif // SIMPLEAUDIOPLAYER_SMTCINTERFACE_H_

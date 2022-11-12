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

private:
    SMTC::SAP_SMTC m_smtc;
};

#endif // SIMPLEAUDIOPLAYER_SMTCINTERFACE_H_

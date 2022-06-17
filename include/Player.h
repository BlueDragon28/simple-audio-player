#ifndef SIMPLEAUDIOPLAYER_PLAYER_H_
#define SIMPLEAUDIOPLAYER_PLAYER_H_

#include <simple-audio-library/AudioPlayer.h>
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QJSEngine>

/*
This singleton class is a wrapper of the simple-audio-library for the QML.
*/
class Player : public QObject
{
    Q_OBJECT

    Player(QObject* parent = nullptr);

public:
    ~Player();

    /*
    Get stream size in frames or in seconds.
    */
    enum SizeType
    {
        FRAMES,
        SECONDS,
    };
    Q_ENUM(SizeType);

    /*
    Static function to instanciate the singleton.
    */
    static QObject* qmlRegistration(QQmlEngine* engine, QJSEngine* jsEngine);

    /*
    Static function to retrieve the singleton instance.
    */
    static Player* instance();

    /*
    Return true if simple-audio-library is initialized.
    */
    Q_INVOKABLE bool isRunning() const;

    /*
    Return true if simple-audio-library is playing an audio file.
    */
    Q_INVOKABLE bool isPlaying() const;

    /*
    Is simple-audio-library is ready to play (or is playing) files.
    */
    Q_INVOKABLE bool isReady() const;

    /*
    Return current stream size.
    */
    Q_INVOKABLE size_t streamSize(SizeType type = FRAMES) const;

    /*
    Return current stream position.
    */
    Q_INVOKABLE size_t streamPos(SizeType type = FRAMES) const;

    /*
    Check if a file is readable by the simple-audio-library.
    */
    Q_INVOKABLE int isReadable(const QString& filePath) const;

    /*
    Return a list of strings of all the available files formats readable by the simple-audio-library.
    */
    Q_INVOKABLE QList<QString> supportedFormats() const;

public slots:
    /*
    Open an audio file.
    */
    void open(const QString& filePath);
    void open(const QStringList& filesPath);

    /*
    Start playing or resuming the stream.
    */
    void play();

    /*
    Pause the stream.
    */
    void pause();

    /*
    Stop the stream.
    */
    void stop();

    /*
    Seek inside the stream to a position.
    */
    void seek(size_t pos, SizeType type = FRAMES);

signals:
    /*
    Signal called when the simple-audio-library start playing a new file.
    */
    void startNewFile(const QString& filePath);

    /*
    Signal called when the simple-audio-library has finish playing a file.
    */
    void endFile(const QString& filePath);

    /*
    Signal called every time the simple-audio-library audio backend (PortAudio) fill is buffer.
    The parameter is the stream position in frames.
    */
    void streamPosChangeInFrames(unsigned long streamPos);

    /*
    Signal called every seconds (when a file is playing).
    The parameter is the stream position in seconds.
    */
    void streamPosChangeInSeconds(unsigned long streamPos);

    /*
    Signal called when the stream is paused.
    */
    void streamPaused();

    /*
    Signal called when the stream start playing (or resuming).
    */
    void streamPlaying();

    /*
    Signal called when the stream stop playing.
    */
    void streamStopping();

    /*
    Signal called when the is ready getter is changed.
    */
    void isReadyChanged(bool isReady);

private:
    /*
    This methods are wrapped between the event system of the simple-audio-libray and Qt.
    */
    void salStartNewFile(const std::string& filePath);
    void salEndFile(const std::string& filePath);
    void salStreamPosChangeInFrames(size_t streamPos);
    void salStreamPosChangeInSeconds(size_t streamPos);
    void salStreamPaused();
    void salStreamPlaying();
    void salStreamStopping();
    void salIsReadyChanged(bool isReady);

    // Instance of SAL.
    SAL::AudioPlayer* m_player;

    // Static variable holding the instance of this class.
    static Player* staticInstance;
};

#endif // SIMPLEAUDIOPLAYER_PLAYER_H_
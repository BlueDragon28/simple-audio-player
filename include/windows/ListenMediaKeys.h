#ifndef SIMPLEAUDIOPLAYER_LISTENMEDIAKEYS_H_
#define SIMPLEAUDIOPLAYER_LISTENMEDIAKEYS_H_

#include <QObject>
#include <Windows.h>
#include <atomic>
#include <thread>

/*
Implement a hook to listen to media key press.
Every time the user will press it, the hook event
will be converted to a qt signal.
*/
class ListenMediaKeys : public QObject
{
    Q_OBJECT

    ListenMediaKeys();
public:
    ~ListenMediaKeys();

    static void init();
    static void deinit();
    static ListenMediaKeys* instance();

signals:
    void playPause();
    void next();
    void previous();

private:
    /*
    Creating the hook and listening for signals.
    */
    void hookLoop();

    /*
    Callback called everytime a key is pressed.
    */
    static LRESULT CALLBACK keyHookCallback(
        int nCode,
        WPARAM wParam,
        LPARAM lParam);

    /*
    Interface method to call qt signals.
    */
    enum class KeySignal
    {
        UNKNOWN = -1,
        PLAY_PAUSE,
        NEXT,
        PREVIOUS
    };
    void handleEvent(KeySignal signal);

    /*
    Return the type of the key signals.
    */
    KeySignal processVKCode(DWORD vkCode);

    std::thread m_hookThread;
    std::atomic<HHOOK> m_hookID;
    std::atomic<DWORD> m_hookThreadID;

    static std::unique_ptr<ListenMediaKeys> _instance;
};

#endif // SIMPLEAUDIOPLAYER_LISTENMEDIAKEYS_H_

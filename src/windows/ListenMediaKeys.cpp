#include "windows/ListenMediaKeys.h"

std::unique_ptr<ListenMediaKeys> ListenMediaKeys::_instance;

void ListenMediaKeys::init()
{
    if (!_instance)
    {
        _instance = std::unique_ptr<ListenMediaKeys>(new ListenMediaKeys());
    }
}

void ListenMediaKeys::deinit()
{
    if (_instance)
    {
        _instance.reset();
    }
}

ListenMediaKeys* ListenMediaKeys::instance()
{
    return _instance.get();
}

ListenMediaKeys::ListenMediaKeys() :
    QObject(),
    m_hookID(nullptr),
    m_hookThreadID(NULL)
{
    // Starting the hook on another thread.
    m_hookThread = std::thread(&ListenMediaKeys::hookLoop, this);
}

ListenMediaKeys::~ListenMediaKeys()
{
    // Send a signal to close the hook loop and close it.
    if (m_hookID != nullptr)
    {
        if (m_hookThreadID != NULL)
        {
            PostThreadMessage(m_hookThreadID, WM_QUIT, NULL, NULL);
        }
        if (m_hookThread.joinable())
        {
            m_hookThread.join();
        }
        UnhookWindowsHookEx(m_hookID);
    }
}

void ListenMediaKeys::hookLoop()
{
    /*
    Initialize the loop and listening to any key press event.
    */

    // Getting the current thread, used to send a signal to exit the loop.
    m_hookThreadID = GetCurrentThreadId();

    // Creating the hook.
    m_hookID = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        keyHookCallback,
        nullptr,
        NULL);

    if (m_hookID == 0)
    {
        return;
    }

    // Listening the signal and process avery key press.
    MSG msg;
    while (GetMessage(&msg, NULL, NULL, NULL) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void ListenMediaKeys::handleEvent(KeySignal signal)
{
    // Check what key signal it is and call the appropriate qt signal.

    switch (signal)
    {
    case KeySignal::PLAY_PAUSE:
    {
        emit playPause();
    } break;

    case KeySignal::NEXT:
    {
        emit next();
    } break;

    case KeySignal::PREVIOUS:
    {
        emit previous();
    } break;

    case KeySignal::UNKNOWN:
    default:
        return;
    }
}

ListenMediaKeys::KeySignal ListenMediaKeys::processVKCode(DWORD vkCode)
{
    switch (vkCode)
    {
    case VK_MEDIA_PLAY_PAUSE:
    {
        return KeySignal::PLAY_PAUSE;
    } break;

    case VK_MEDIA_NEXT_TRACK:
    {
        return KeySignal::NEXT;
    } break;

    case VK_MEDIA_PREV_TRACK:
    {
        return KeySignal::PREVIOUS;
    } break;

    default:
    {
        return KeySignal::UNKNOWN;
    } break;
    }
}

LRESULT CALLBACK ListenMediaKeys::keyHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    /*
    This is the hook callback, every time a key is pressed this function is called.
    We process it and check if a media key has been pressed.
    */
    if (nCode < 0)
    {
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    switch(wParam)
    {
    case WM_KEYDOWN:
    {
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        instance()->handleEvent(instance()->processVKCode(p->vkCode));
    } break;
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

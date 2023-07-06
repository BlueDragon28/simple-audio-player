#ifndef SIMPLEAUDIOPLAYER_EVENT_CALLBACK_H_
#define SIMPLEAUDIOPLAYER_EVENT_CALLBACK_H_

#include <functional>

class EventCallback
{
public:
    EventCallback();
    ~EventCallback();

    bool isValid() const;
    void call();
    void setCallback(const std::function<void()>& function);

private:
    bool m_isValid;
    std::function<void()> m_callback;
};

#endif // SIMPLEAUDIOPLAYER_EVENT_CALLBACK_H_

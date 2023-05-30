#include "spotify/EventCallback.h"

EventCallback::EventCallback() :
    m_isValid(false)
{}

EventCallback::~EventCallback()
{}

bool EventCallback::isValid() const
{
    return m_isValid;
}

void EventCallback::call()
{
    if (!m_isValid) return;

    m_callback();
    m_callback = std::function<void()>();
    m_isValid = false;
}

void EventCallback::setCallback(const std::function<void()>& function)
{
    m_callback = function;
    m_isValid = true;
}


#include <algorithm>
#include <iterator>

#include "NearObjectSession.hxx"

using namespace nearobject;

NearObjectSession::~NearObjectSession()
{
    OnSessionClosed();
}

std::vector<std::shared_ptr<NearObjectSessionEventCallbacks>> 
NearObjectSession::GetEventCallbacks()
{
    std::vector<std::shared_ptr<NearObjectSessionEventCallbacks>> eventCallbacks{};
    const auto lock = std::scoped_lock{ m_eventCallbacksGate };

    for (auto& eventCallbackWeak : m_eventCallbacks)
    {
        if (auto eventCallback = eventCallbackWeak.lock())
        {
            eventCallbacks.push_back(std::move(eventCallback));
        }
    }

    return eventCallbacks;
}

void
NearObjectSession::OnSessionClosed()
{
    auto eventCallbacks = GetEventCallbacks();
    for (auto& eventCallback : eventCallbacks)
    {
        eventCallback->SessionEnded();
    }
}

void
NearObjectSession::RegisterCallbacks(std::weak_ptr<NearObjectSessionEventCallbacks> eventCallback)
{
    const auto lock = std::scoped_lock{ m_eventCallbacksGate };
    m_eventCallbacks.push_back(eventCallback);
}

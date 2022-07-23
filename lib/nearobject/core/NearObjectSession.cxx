
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
    const auto lock = std::scoped_lock{ m_eventCallbacksGate };

    // Attempt to resolve each pointer, adding a copy to the return vector if
    // successful, removing it from the registered callback vector otherwise.
    std::vector<std::shared_ptr<NearObjectSessionEventCallbacks>> eventCallbacks{};
    for (auto it = std::begin(m_eventCallbacks); it != std::end(m_eventCallbacks); it = std::next(it))
    {
        if (const auto eventCallback = it->lock())
        {
            eventCallbacks.push_back(eventCallback);
        }
        else
        {
            m_eventCallbacks.erase(it);
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

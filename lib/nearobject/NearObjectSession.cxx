
#include <algorithm>
#include <iterator>

#include "NearObjectSession.hxx"
#include "NearObjectSessionEventCallbacks.hxx"

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
    for (auto it = std::begin(m_eventCallbacks); it != std::end(m_eventCallbacks); ) {
        if (const auto eventCallback = it->lock()) {
            eventCallbacks.push_back(eventCallback);
            it = std::next(it);
        } else {
            it = m_eventCallbacks.erase(it);
        }
    }

    return eventCallbacks;
}

void
NearObjectSession::OnSessionClosed()
{
    // TODO: All callbacks should probably be serialized to ensure that events
    // firing in rapid succession don't get signaled out-of-order. Likely some
    // form of a queue is needed.
    StopRangingSession();

    for (const auto& eventCallback : GetEventCallbacks()) {
        eventCallback->OnNearObjectSessionEnded(this);
    }
}

NearObjectSession::StartRangingSessionResult
NearObjectSession::StartRangingSession()
{
    StartRangingSessionResult result;

    const auto lock = std::scoped_lock{ m_rangingStateGate };
    if (m_rangingSessionActive) {
        result.Status = RangingSessionStatus::MaximumSessionsReached;
    } else {
        result.Status = CreateNewRangingSession();
        m_rangingSessionActive = (result.Status == RangingSessionStatus::Running);
    }

    return result;
}

NearObjectSession::RangingSessionStatus
NearObjectSession::CreateNewRangingSession()
{
    if (!IsRangingSupported()) {
        return RangingSessionStatus::NotSupported;
    }

    {
        const auto lock = std::scoped_lock{ m_rangingStateGate };
        // TODO: actually create new ranging session
        m_rangingSessionActive = true;

    }

    for (const auto& eventCallback : GetEventCallbacks()) {
        eventCallback->OnNearObjectRangingSessionStarted(this);
    }

    return RangingSessionStatus::Running;
}

void
NearObjectSession::StopRangingSession()
{
    const auto lock = std::scoped_lock{ m_rangingStateGate };
    if (!m_rangingSessionActive) {
        return;
    }

    // TODO: signal to device to stop ranging

    m_rangingSessionActive = false;

    for (const auto& eventCallback : GetEventCallbacks()) {
        eventCallback->OnNearObjectRangingSessionEnded(this);
    }
}

bool
NearObjectSession::IsRangingActive() const noexcept
{
    const auto lock = std::scoped_lock{ m_rangingStateGate };
    return m_rangingSessionActive;
}

bool
NearObjectSession::IsRangingSupported() const noexcept
{
    // TODO: check session capabilities and return actual support
    return true;
}

void
NearObjectSession::RegisterCallbacks(std::weak_ptr<NearObjectSessionEventCallbacks> eventCallback)
{
    const auto lock = std::scoped_lock{ m_eventCallbacksGate };
    m_eventCallbacks.push_back(eventCallback);
}

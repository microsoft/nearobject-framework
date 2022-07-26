
#include <algorithm>
#include <iterator>

#include "NearObjectSession.hxx"
#include "NearObjectSessionEventCallbacks.hxx"

using namespace nearobject;

NearObjectSession::NearObjectSession(NearObjectCapabilities capabilities, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) :
    Capabilities(capabilities),
    m_eventCallbacks(eventCallbacks)
{}

NearObjectSession::~NearObjectSession()
{
    EndSession();
}

void
NearObjectSession::EndSession()
{
    // TODO: All callbacks should probably be serialized to ensure that events
    // firing in rapid succession don't get signaled out-of-order. Likely some
    // form of a queue is needed.
    StopRangingSession();

    if (const auto eventCallbacks  = m_eventCallbacks.lock()) {
        eventCallbacks->OnNearObjectSessionEnded(this);
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

    if (const auto eventCallbacks = m_eventCallbacks.lock()) {
        eventCallbacks->OnNearObjectRangingSessionStarted(this);
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

    if (const auto eventCallbacks = m_eventCallbacks.lock()) {
        eventCallbacks->OnNearObjectRangingSessionEnded(this);
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

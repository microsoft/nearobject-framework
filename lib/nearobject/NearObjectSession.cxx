
#include <algorithm>
#include <iterator>

#include "NearObjectSession.hxx"
#include "NearObjectSessionEventCallbacks.hxx"

using namespace nearobject;

NearObjectSession::NearObjectSession(NearObjectCapabilities capabilities, const std::vector<std::shared_ptr<NearObject>> nearObjectPeers, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) :
    Capabilities(capabilities),
    m_nearObjectPeers(nearObjectPeers),
    m_eventCallbacks(eventCallbacks)
{ }

NearObjectSession::~NearObjectSession()
{
    EndSession();
}

bool
NearObjectSession::InvokeEventCallback(const std::function<void(NearObjectSessionEventCallbacks& callbacks)>& executor)
{
    // TODO: serialize all requests
    // TODO: make this async (caller should not have to wait)

    const auto eventCallbacks = m_eventCallbacks.lock();
    if (!eventCallbacks) {
        return false;
    }

    executor(*eventCallbacks);
    return true;
} 

bool
NearObjectSession::AddNearObjectPeer(const std::shared_ptr<NearObject> nearObjectAdded)
{
    const auto lock = std::scoped_lock{ m_nearObjectPeersGate };
    const auto nearObjectAlreadyMember = std::any_of(std::cbegin(m_nearObjectPeers), std::cend(m_nearObjectPeers), [&](const auto nearObject)
    {
        return (*nearObject == *nearObjectAdded);
    });

    if (nearObjectAlreadyMember) {
        return false;
    }

    m_nearObjectPeers.push_back(nearObjectAdded);
    InvokeEventCallback([&](auto& eventCallbacks){
        eventCallbacks.OnNearObjectSessionMembershipChanged(this, { nearObjectAdded }, {});
    });

    return true;
}

bool
NearObjectSession::RemoveNearObjectPeer(const std::shared_ptr<NearObject> nearObjectRemoved)
{
    const auto lock = std::scoped_lock{ m_nearObjectPeersGate };
    const auto nearObjectToRemove = std::find_if(std::cbegin(m_nearObjectPeers), std::cend(m_nearObjectPeers), [&](const auto& nearObject)
    {
        return (*nearObject == *nearObjectRemoved);
    });

    if (nearObjectToRemove == std::cend(m_nearObjectPeers)) {
        return false;
    }

    // No need to take an extra ref since the function argument guarantees
    // its lifetime throughout this function.
    m_nearObjectPeers.erase(nearObjectToRemove);
    InvokeEventCallback([&](auto& eventCallbacks){
        eventCallbacks.OnNearObjectSessionMembershipChanged(this, {}, { nearObjectRemoved });
    });

    return true;
}

void
NearObjectSession::EndSession()
{
    // TODO: All callbacks should probably be serialized to ensure that events
    // firing in rapid succession don't get signaled out-of-order. Likely some
    // form of a queue is needed.
    StopRangingSession();

    InvokeEventCallback([&](auto& eventCallbacks) {
        eventCallbacks.OnNearObjectSessionEnded(this);
    });
}

void
NearObjectSession::NearObjectPropertiesChanged(const std::shared_ptr<NearObject> nearObjectChanged)
{
    InvokeEventCallback([&](auto& eventCallbacks){
        eventCallbacks.OnNearObjectSessionNearObjectPropertiesChanged(this, { nearObjectChanged });
    });
}

NearObjectSession::StartRangingSessionResult
NearObjectSession::StartRangingSession()
{
    StartRangingSessionResult result;

    const auto lock = std::scoped_lock{ m_rangingStateGate };
    if (m_rangingSession.has_value()) {
        result.Status = RangingSessionStatus::MaximumSessionsReached;
    } else {
        result.Status = CreateNewRangingSession();
    }

    return result;
}

NearObjectSession::RangingSessionStatus
NearObjectSession::CreateNewRangingSession()
{
    if (!Capabilities.SupportsRanging) {
        return RangingSessionStatus::NotSupported;
    }

    RangingSession rangingSession{ [&]() {
        // TODO
    } };

    // TODO: actually create new ranging session
    m_rangingSession = std::move(rangingSession);

    InvokeEventCallback([&](auto& eventCallbacks){
        eventCallbacks.OnNearObjectRangingSessionStarted(this);
    });

    return RangingSessionStatus::Running;
}

void
NearObjectSession::StopRangingSession()
{
    const auto lock = std::scoped_lock{ m_rangingStateGate };
    if (!m_rangingSession.has_value()) {
        return;
    }

    // TODO: signal to device to stop ranging
    m_rangingSession.reset();

    InvokeEventCallback([&](auto& eventCallbacks){
        eventCallbacks.OnNearObjectRangingSessionEnded(this);
    });
}

bool
NearObjectSession::IsRangingActive() const noexcept
{
    const auto lock = std::scoped_lock{ m_rangingStateGate };
    return m_rangingSession.has_value();
}

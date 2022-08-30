
#include <algorithm>
#include <iterator>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject;

NearObjectSession::NearObjectSession(NearObjectCapabilities capabilities, const std::vector<std::shared_ptr<NearObject>>& nearObjectPeers, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) :
    m_capabilities(capabilities),
    m_nearObjectPeers(nearObjectPeers),
    m_eventCallbacks(std::move(eventCallbacks)),
    m_taskQueue()
{
    m_taskQueue.run();
}

NearObjectSession::~NearObjectSession()
{
    EndSession();
}

NearObjectCapabilities
NearObjectSession::GetCapabilities() const noexcept
{
    return m_capabilities;
}

void
NearObjectSession::InvokeEventCallback(const std::function<void(NearObjectSessionEventCallbacks& callbacks)> executor)
{
    auto dispatcher = m_taskQueue.getDispatcher();

    auto const task = [this, executor]() {
        const auto eventCallbacks = m_eventCallbacks.lock();
        if (!eventCallbacks) {
            return;
        }
        executor(*eventCallbacks);
    };

    dispatcher->post(std::move(task));
}

void
NearObjectSession::InvokeBlockingEventCallback(const std::function<void(NearObjectSessionEventCallbacks& callbacks)> executor)
{
    auto dispatcher = m_taskQueue.getDispatcher();

    auto const task = [this, &executor]() {
        const auto eventCallbacks = m_eventCallbacks.lock();
        if (!eventCallbacks) {
            return;
        }
        executor(*eventCallbacks);
    };

    dispatcher->postBlocking(std::move(task));
}

void
NearObjectSession::AddNearObjectPeer(std::shared_ptr<NearObject> nearObjectAdded)
{
    AddNearObjectPeers({ std::move(nearObjectAdded) });
}

void
NearObjectSession::AddNearObjectPeers(std::vector<std::shared_ptr<NearObject>> nearObjectsToAdd)
{
    const auto lock = std::scoped_lock{ m_nearObjectPeersGate };

    // Remove objects already in the peer set from the list of objects to add.
    // The original vector 'nearObjectsToAdd' is maintained such that it can be
    // passed to the membership changed callback later, alleviating making a
    // copy of these elements.
    nearObjectsToAdd.erase(std::remove_if(std::begin(nearObjectsToAdd), std::end(nearObjectsToAdd), [&](const auto& nearObjectPeerToAdd) {
        return std::any_of(std::cbegin(m_nearObjectPeers), std::cend(m_nearObjectPeers), [&](const auto& nearObjectPeer) {
            return (*nearObjectPeer == *nearObjectPeerToAdd);
        });
    }), std::end(nearObjectsToAdd));

    // Add each peer from the pruned list to the existing peers.
    m_nearObjectPeers.insert(std::end(m_nearObjectPeers), std::cbegin(nearObjectsToAdd), std::cend(nearObjectsToAdd));

    // Signal the membership changed event with the added peers.
    InvokeEventCallback([this, nearObjectsToAdd = std::move(nearObjectsToAdd)](auto& eventCallbacks) {
        eventCallbacks.OnSessionMembershipChanged(this, std::move(nearObjectsToAdd), {});
    });
}

void
NearObjectSession::RemoveNearObjectPeer(std::shared_ptr<NearObject> nearObjectRemoved)
{
    RemoveNearObjectPeers({ std::move(nearObjectRemoved) });
}

void
NearObjectSession::RemoveNearObjectPeers(std::vector<std::shared_ptr<NearObject>> nearObjectsToRemove)
{
    const auto nearObjectPeersLock = std::scoped_lock{ m_nearObjectPeersGate };

    // Partition the existing peers into ones that should be kept (first
    // partition) and ones that should be removed (second partition), keeping
    // their relative order (stable). std::stable_partition returns an iterator
    // to the beginning of the second partition.
    const auto nearObjectsRemoved = std::stable_partition(std::begin(m_nearObjectPeers), std::end(m_nearObjectPeers), [&](const auto nearObjectToCheck) { 
        return std::none_of(std::cbegin(nearObjectsToRemove), std::cend(nearObjectsToRemove), [&](const auto& nearObjectToRemove){
            return (nearObjectToCheck == nearObjectToRemove);
        });
    });

    // Move the peers that should be removed into the nearObjectsToRemove vector to preserve them.
    nearObjectsToRemove = {
        std::make_move_iterator(nearObjectsRemoved),
        std::make_move_iterator(std::end(m_nearObjectPeers))
    };

    // Erase the moved-from peers from the existing peer list.
    m_nearObjectPeers.erase(nearObjectsRemoved, std::end(m_nearObjectPeers));

    // Signal the membership changed event with the removed peers.
    InvokeEventCallback([this, nearObjectsToRemove = std::move(nearObjectsToRemove)](auto& eventCallbacks) {
        eventCallbacks.OnSessionMembershipChanged(this, {}, std::move(nearObjectsToRemove));
    });
}

void
NearObjectSession::EndSession()
{
    // TODO: All callbacks should probably be serialized to ensure that events
    // firing in rapid succession don't get signaled out-of-order. Likely some
    // form of a queue is needed.
    StopRanging();

    InvokeBlockingEventCallback([&](auto& eventCallbacks) {
        eventCallbacks.OnSessionEnded(this);
    });
}

void
NearObjectSession::NearObjectPropertiesChanged(const std::shared_ptr<NearObject> nearObjectChanged)
{
    InvokeEventCallback([this, nearObjectChanged](auto& eventCallbacks) {
        eventCallbacks.OnNearObjectPropertiesChanged(this, { nearObjectChanged });
    });
}

std::vector<std::shared_ptr<NearObject>>
NearObjectSession::GetPeers() const noexcept
{
    const auto nearObjectPeersLock = std::scoped_lock{ m_nearObjectPeersGate };
    return m_nearObjectPeers;
}

NearObjectSession::StartRangingSessionResult
NearObjectSession::StartRanging()
{
    StartRangingSessionResult result{};

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
    if (!m_capabilities.SupportsRanging) {
        return RangingSessionStatus::NotSupported;
    }

    RangingSession rangingSession{ [&]() {
        // TODO
    } };

    // TODO: actually create new ranging session
    m_rangingSession.emplace(std::move(rangingSession));

    InvokeEventCallback([&](auto& eventCallbacks) {
        eventCallbacks.OnRangingStarted(this);
    });

    return RangingSessionStatus::Running;
}

void
NearObjectSession::StopRanging()
{
    const auto lock = std::scoped_lock{ m_rangingStateGate };
    if (!m_rangingSession.has_value()) {
        return;
    }

    // TODO: signal to device to stop ranging
    m_rangingSession.reset();

    InvokeBlockingEventCallback([&](auto& eventCallbacks) {
        eventCallbacks.OnRangingStopped(this);
    });
}

bool
NearObjectSession::IsRangingActive() const noexcept
{
    const auto lock = std::scoped_lock{ m_rangingStateGate };
    return m_rangingSession.has_value();
}

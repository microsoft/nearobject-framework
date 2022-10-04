
#include <algorithm>
#include <iterator>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>
#include <chrono>

using namespace std::chrono_literals;
using namespace nearobject;

NearObjectSession::NearObjectSession(uint32_t sessionId, NearObjectCapabilities capabilities, const std::vector<std::shared_ptr<NearObject>>& nearObjects, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) :
    m_sessionId(sessionId),
    m_capabilities(capabilities),
    m_nearObjects(nearObjects),
    m_eventCallbacks(std::move(eventCallbacks))
{
}

NearObjectSession::~NearObjectSession()
{
    EndSession();
}

uint32_t
NearObjectSession::GetId() const noexcept
{
    return m_sessionId;
}

NearObjectCapabilities
NearObjectSession::GetCapabilities() const noexcept
{
    return m_capabilities;
}

void
NearObjectSession::InvokeEventCallback(std::function<void(NearObjectSessionEventCallbacks& callbacks)> executor)
{
    auto dispatcher = m_taskQueue.get_dispatcher();

    auto const task = [this, executor = std::move(executor)]() {
        const auto eventCallbacks = m_eventCallbacks.lock();
        if (!eventCallbacks) {
            return;
        }
        executor(*eventCallbacks);
    };

    dispatcher->post(std::move(task));
}

void
NearObjectSession::AddNearObject(std::shared_ptr<NearObject> nearObjectAdded)
{
    AddNearObjects({ std::move(nearObjectAdded) });
}

void
NearObjectSession::AddNearObjects(std::vector<std::shared_ptr<NearObject>> nearObjectsToAdd)
{
    const auto lock = std::scoped_lock{ m_nearObjectsGate };

    // Remove objects already in the member set from the list of objects to add.
    // The original vector 'nearObjectsToAdd' is maintained such that it can be
    // passed to the membership changed callback later, alleviating making a
    // copy of these elements.
    std::erase_if(nearObjectsToAdd, [&](const auto& nearObject) {
        return std::any_of(std::cbegin(m_nearObjects), std::cend(m_nearObjects), [&](const auto& nearObjectToAdd) {
            return (*nearObject == *nearObjectToAdd);
        });
    });

    // Add each near object from the pruned list to the existing near objects.
    m_nearObjects.insert(std::end(m_nearObjects), std::cbegin(nearObjectsToAdd), std::cend(nearObjectsToAdd));

    // Signal the membership changed event with the added near objects.
    InvokeEventCallback([this, nearObjectsToAdd = std::move(nearObjectsToAdd)](auto& eventCallbacks) {
        eventCallbacks.OnSessionMembershipChanged(this, nearObjectsToAdd, {});
    });
}

void
NearObjectSession::RemoveNearObject(std::shared_ptr<NearObject> nearObjectRemoved)
{
    RemoveNearObjects({ std::move(nearObjectRemoved) });
}

void
NearObjectSession::RemoveNearObjects(std::vector<std::shared_ptr<NearObject>> nearObjectsToRemove)
{
    const auto nearObjectsLock = std::scoped_lock{ m_nearObjectsGate };

    // Partition the existing near objects into ones that should be kept (first
    // partition) and ones that should be removed (second partition), keeping
    // their relative order (stable). std::stable_partition returns an iterator
    // to the beginning of the second partition.
    auto nearObjectsRemoved = std::stable_partition(std::begin(m_nearObjects), std::end(m_nearObjects), [&](const auto nearObjectToCheck) { 
        return std::none_of(std::cbegin(nearObjectsToRemove), std::cend(nearObjectsToRemove), [&](const auto& nearObjectToRemove){
            return (nearObjectToCheck == nearObjectToRemove);
        });
    });

    // Move the near objects that should be removed into the nearObjectsToRemove vector to preserve them.
    nearObjectsToRemove = {
        std::make_move_iterator(nearObjectsRemoved),
        std::make_move_iterator(std::end(m_nearObjects))
    };

    // Erase the moved-from near objects from the existing membership list.
    m_nearObjects.erase(nearObjectsRemoved, std::end(m_nearObjects));

    // Signal the membership changed event with the removed near objects.
    InvokeEventCallback([this, nearObjectsToRemove = std::move(nearObjectsToRemove)](auto& eventCallbacks) {
        eventCallbacks.OnSessionMembershipChanged(this, {}, nearObjectsToRemove);
    });
}

void
NearObjectSession::EndSession()
{
    StopRanging();

    // the blocking version is called so that the task queue can execute the task before *this is destructed
    InvokeEventCallback([&](auto& eventCallbacks) {
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
NearObjectSession::GetNearObjects() const noexcept
{
    const auto nearObjectsLock = std::scoped_lock{ m_nearObjectsGate };
    return m_nearObjects;
}

NearObjectSession::RangingSessionStatus
NearObjectSession::StartRanging()
{
    RangingSessionStatus status = RangingSessionStatus::Error;

    const auto lock = std::scoped_lock{ m_rangingStateGate };
    if (m_isRangingActive) {
        status = RangingSessionStatus::MaximumSessionsReached;
    } else {
        status = CreateNewRangingSession();
    }

    return status;
}

NearObjectSession::RangingSessionStatus
NearObjectSession::CreateNewRangingSession()
{
    if (!m_capabilities.SupportsRanging) {
        return RangingSessionStatus::NotSupported;
    }

    // TODO: actually create and configure new ranging session
    m_isRangingActive = true;

    InvokeEventCallback([&](auto& eventCallbacks) {
        eventCallbacks.OnRangingStarted(this);
    });

    return RangingSessionStatus::Running;
}

void
NearObjectSession::StopRanging()
{
    const auto lock = std::scoped_lock{ m_rangingStateGate };
    if (!m_isRangingActive) {
        return;
    }

    // TODO: signal to device to stop ranging
    m_isRangingActive = false;

    InvokeEventCallback([&](auto& eventCallbacks) {
        eventCallbacks.OnRangingStopped(this);
    });
}

bool
NearObjectSession::IsRangingActive() const noexcept
{
    const auto lock = std::scoped_lock{ m_rangingStateGate };
    return m_isRangingActive;
}

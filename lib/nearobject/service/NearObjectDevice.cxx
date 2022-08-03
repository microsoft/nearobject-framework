
#include "NearObjectDevice.hxx"

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

NearObjectDevice::NearObjectDevice(uint64_t deviceId) :
    m_deviceId{ deviceId }
{}

uint64_t
NearObjectDevice::Id() const noexcept
{
    return m_deviceId;
}

NearObjectDevice::StartSessionResult
NearObjectDevice::StartSession(const NearObjectProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks)
{
    auto result = StartSessionImpl(profile, std::move(eventCallbacks));

    // Store a weak_ptr to each active session for tracking purposes.
    if (result.Session.has_value()) {
        const auto sessionsLock = std::scoped_lock{ m_sessionsGate };
        m_sessions.push_back(result.Session.value());
    } else {
        // TODO: log error details
    }

    return result;
}

bool
nearobject::service::operator==(const NearObjectDevice& lhs, const NearObjectDevice& rhs) noexcept
{
    return lhs.Id() == rhs.Id();
}

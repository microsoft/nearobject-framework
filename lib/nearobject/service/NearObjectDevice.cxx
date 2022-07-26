
#include "NearObjectDevice.hxx"

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

NearObjectDevice::StartSessionResult
NearObjectDevice::StartSession(const NearObjectConnectionProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks)
{
    auto result = StartSessionImpl(profile, eventCallbacks);

    // Store a weak_ptr to each active session for tracking purposes.
    if (result.Session.has_value()) {
        const auto sessionsLock = std::scoped_lock{ m_sessionsGate };
        m_sessions.push_back(result.Session.value());
    } else {
        // TODO: log error details
    }
    
    return result;
}


#include <nearobject/service/NearObjectDeviceController.hxx>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

NearObjectDeviceController::StartSessionResult
NearObjectDeviceController::StartSession(const NearObjectProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks)
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
nearobject::service::operator==(const NearObjectDeviceController& lhs, const NearObjectDeviceController& rhs) noexcept
{
    // Ensure types are the same, then compare using overidden equality
    // function. This guarantees that the overriden IsEqual() member function
    // always receives instances of the same type, allowing it to be safely
    // converted using static_cast.
    return (typeid(lhs) == typeid(rhs)) && lhs.IsEqual(rhs);
}

bool
nearobject::service::operator!=(const NearObjectDeviceController& lhs, const NearObjectDeviceController& rhs) noexcept
{
    return !(lhs == rhs);
}

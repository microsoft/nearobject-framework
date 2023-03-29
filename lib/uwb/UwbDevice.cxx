
#include <future>
#include <type_traits>
#include <variant>

#include <magic_enum.hpp>
#include <plog/Log.h>
#include <ranges>
#include <uwb/UwbDevice.hxx>

using namespace uwb;
using namespace uwb::protocol::fira;

std::shared_ptr<UwbSession>
UwbDevice::GetSession(uint32_t sessionId)
{
    // First determine if the session is already in the cache.
    auto session = FindSession(sessionId);
    if (session != nullptr) {
        return session;
    }

    // Session not in cache. Attempt to resolve the session from the driver.
    session = ResolveSessionImpl(sessionId);
    if (session == nullptr) {
        // TODO: the ResolveSessionImpl function should probably return or throw
        // UwbException such that this code can differentiate between a
        // completed call where no session exists, and a failed call where
        // the resolution failed and so could not produce a UwbSession object
        // whether the session existed or not.

        // No such session exists.
        return session;
    }

    // Re-check if the session is in the cache. An exclusive lock is taken here
    // early to check for a race where another caller could have invoked this
    // function concurrently, resolved the session, and added it to the cache.
    std::unique_lock sessionExclusiveLock{ m_sessionsGate };

    // Check if a concurrent call resolved this device concurrently and won the
    // race to cache it.
    auto sessionCached = FindSessionLocked(sessionId);
    if (sessionCached != nullptr) {
        return sessionCached;
    }

    // Add the resolved session to the cache. Note that this instance will not
    // have any session callbacks attached to it since it was constructed by the
    // lower layer and no callbacks are taken as input to this function. This is
    // fine since the callbacks aren't used by this class; instead, clients that
    // obtain an instance to this session via GetSession() must set callbacks on
    // it themselves, if desired.
    m_sessions[session->GetId()] = std::weak_ptr<UwbSession>(session);
    return session;
}

std::shared_ptr<UwbSession>
UwbDevice::FindSession(uint32_t sessionId)
{
    std::shared_lock sessionSharedLock{ m_sessionsGate };
    auto session = FindSessionLocked(sessionId);
    return session;
}

std::shared_ptr<UwbSession>
UwbDevice::FindSessionLocked(uint32_t sessionId)
{
    auto sessionIt = m_sessions.find(sessionId);
    if (sessionIt == std::cend(m_sessions)) {
        PLOG_VERBOSE << "Session with id=" << sessionId << " not found in cache";
        return nullptr;
    }

    auto sessionWeak = sessionIt->second;
    auto session = sessionWeak.lock();
    if (!session) {
        PLOG_VERBOSE << "Session with id=" << sessionId << " expired from cache";
        return nullptr;
    }

    return session;
}

void
UwbDevice::OnStatusChanged(UwbStatus status)
{
    m_lastError = status;
    std::visit([&status](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_enum_v<T>) {
            PLOG_VERBOSE << magic_enum::enum_name(arg);
        } else {
            PLOG_WARNING << "unknown UwbStatus variant value encountered";
        }
    },
        status);
}

void
UwbDevice::OnDeviceStatusChanged(UwbStatusDevice statusDevice)
{
    if (m_status.State == UwbDeviceState::Uninitialized) {
        m_status = statusDevice;
        PLOG_VERBOSE << "initialized device status as " << m_status.ToString();
        return;
    }
    if (m_status.State != statusDevice.State) {
        PLOG_VERBOSE << "changed state: " << m_status.ToString() << " --> " << statusDevice.ToString();
        m_status = statusDevice;
    }
}

void
UwbDevice::OnSessionStatusChanged(UwbSessionStatus statusSession)
{
    // TODO should this be something that the session handles itself?
    auto session = FindSession(statusSession.SessionId);
    if (!session) {
        PLOG_WARNING << "Ignoring StatusChanged event due to missing session";
        return;
    }

    session->SetSessionStatus(statusSession);
}

std::shared_ptr<UwbSession>
UwbDevice::CreateSession(uint32_t sessionId, std::weak_ptr<UwbSessionEventCallbacks> callbacks)
{
    auto session = CreateSessionImpl(sessionId, std::move(callbacks));
    {
        std::unique_lock sessionExclusiveLock{ m_sessionsGate };
        m_sessions[session->GetId()] = std::weak_ptr<UwbSession>(session);
    }

    return session;
}

UwbCapability
UwbDevice::GetCapabilities()
{
    PLOG_INFO << "GetCapabilities()";
    return GetCapabilitiesImpl();
}

/**
 * @brief Get the FiRa device information of the device.
 *
 * @return ::uwb::protocol::fira::UwbDeviceInformation
 */
UwbDeviceInformation
UwbDevice::GetDeviceInformation()
{
    PLOG_INFO << "GetDeviceInformation";
    return GetDeviceInformationImpl();
}

void
UwbDevice::Reset()
{
    PLOG_INFO << "Reset";
    ResetImpl();
}

bool
UwbDevice::Initialize()
{
    PLOG_INFO << "Initialize";
    return InitializeImpl();
}

bool
UwbDevice::InitializeImpl()
{
    return true;
}

bool
uwb::operator==(const UwbDevice& lhs, const UwbDevice& rhs) noexcept
{
    return (typeid(lhs) == typeid(rhs)) && lhs.IsEqual(rhs);
}

bool
uwb::operator!=(const UwbDevice& lhs, const UwbDevice& rhs) noexcept
{
    return !(lhs == rhs);
}

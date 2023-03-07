
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
    std::weak_ptr<UwbSession> sessionWeak;
    {
        std::shared_lock sessionSharedLock{ m_sessionsGate };
        auto sessionIt = m_sessions.find(sessionId);
        if (sessionIt == std::cend(m_sessions)) {
            PLOG_VERBOSE << "Session with id=" << sessionId << " not found";
            return nullptr;
        }

        sessionWeak = sessionIt->second;
    }

    auto session = sessionWeak.lock();
    if (!session) {
        PLOG_VERBOSE << "Session with id=" << sessionId << " expired";
        return nullptr;
    }

    return session;
}

void
UwbDevice::AddCallbacks(UwbDeviceEventCallbacks callbacks){
    m_callbacks.push_back(callbacks);
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
    for (const auto& callback : m_callbacks) {
        callback.OnStatusChanged(status);
    }
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
    for (const auto& callback : m_callbacks) {
        callback.OnDeviceStatusChanged(statusDevice);
    }
}

void
UwbDevice::OnSessionStatusChanged(UwbSessionStatus statusSession)
{
    auto session = GetSession(statusSession.SessionId);
    if (!session) {
        PLOG_WARNING << "Ignoring StatusChanged event due to missing session";
        return;
    }

    session->SetSessionStatus(statusSession);
    for (const auto& callback : m_callbacks) {
        callback.OnSessionStatusChanged(statusSession); // TODO session needs to register itself here
    }
}

std::shared_ptr<UwbSession>
UwbDevice::CreateSession()
{
    auto session = CreateSessionImpl();
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

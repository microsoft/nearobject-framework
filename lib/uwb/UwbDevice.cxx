
#include <type_traits>
#include <variant>

#include <plog/Log.h>
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
UwbDevice::OnStatusChanged([[maybe_unused]] UwbStatus status)
{
    // TODO: implement this
}

void
UwbDevice::OnDeviceStatusChanged([[maybe_unused]] UwbStatusDevice statusDevice)
{
    // TODO: implement this
}

void
UwbDevice::OnSessionStatusChanged(UwbSessionStatus statusSession)
{
    auto session = GetSession(statusSession.SessionId);
    if (!session) {
        PLOG_WARNING << "Ignoring StatusChanged event due to missing session";
        return;
    }

    // TODO: implement this
}

void
UwbDevice::OnSessionMulticastListStatus(UwbSessionUpdateMulicastListStatus statusMulticastList)
{
    auto session = GetSession(statusMulticastList.SessionId);
    if (!session) {
        PLOG_WARNING << "Ignoring MulticastListStatus event due to missing session";
        return;
    }

    // TODO: implement this
}

void
UwbDevice::OnSessionRangingData(UwbRangingData rangingData)
{
    auto session = GetSession(rangingData.SessionId);
    if (!session) {
        PLOG_WARNING << "Ignoring RangingData event due to missing session";
        return;
    }

    // TODO: implement this
}

void
UwbDevice::OnUwbNotification(UwbNotificationData uwbNotificationData)
{
    std::visit([this](auto&& arg) {
        using ValueType = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<ValueType, UwbStatus>) {
            OnStatusChanged(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbStatusDevice>) {
            OnDeviceStatusChanged(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionStatus>) {
            OnSessionStatusChanged(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionUpdateMulicastListStatus>) {
            OnSessionMulticastListStatus(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbRangingData>) {
            OnSessionRangingData(arg);
        }
    },
        uwbNotificationData);
}

std::shared_ptr<UwbSession>
UwbDevice::CreateSession(std::weak_ptr<UwbSessionEventCallbacks> callbacks)
{
    auto session = CreateSessionImpl(callbacks);
    {
        std::unique_lock sessionExclusiveLock{ m_sessionsGate };
        m_sessions[session->GetId()] = std::weak_ptr<UwbSession>(session);
    }

    return session;
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

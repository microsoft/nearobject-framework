
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
    auto session = GetSession(statusSession.SessionId);
    if (!session) {
        PLOG_WARNING << "Ignoring StatusChanged event due to missing session";
        return;
    }

    session->SetSessionStatus(statusSession);
}

void
UwbDevice::OnSessionMulticastListStatus(UwbSessionUpdateMulicastListStatus statusMulticastList)
{
    auto session = GetSession(statusMulticastList.SessionId);
    if (!session) {
        PLOG_WARNING << "Ignoring MulticastListStatus event due to missing session";
        return;
    }

    auto peersAdded = std::views::filter(statusMulticastList.Status, [](const auto& peerAddStatus) -> bool {
        return peerAddStatus.Status == UwbStatusMulticast::OkUpdate;
    });

    PLOG_VERBOSE << "Session with id " << statusMulticastList.SessionId << " adding peers";
    session->InsertPeers(peersAdded);

    // Now log the bad status
    IF_PLOG(plog::verbose)
    {
        for (const auto& peer : statusMulticastList.Status) {
            if (peer.Status != UwbStatusMulticast::OkUpdate) {
                PLOG_VERBOSE << "peer has bad status: " << peer.ToString();
            }
        }
    }
}

void
UwbDevice::OnSessionRangingData(UwbRangingData rangingData)
{
    auto session = GetSession(rangingData.SessionId);
    if (!session) {
        PLOG_WARNING << "Ignoring RangingData event due to missing session";
        return;
    }

    PLOG_VERBOSE << "Session with id " << rangingData.SessionId << " processing new ranging data";
    std::vector<UwbPeer> peersData;
    peersData.reserve(rangingData.RangingMeasurements.size());
    for (const auto& peerData : rangingData.RangingMeasurements) {
        UwbPeer data{ peerData };
        PLOG_VERBOSE << "Peer data: " << data.ToString();
        peersData.push_back(std::move(data));
    }
    session->ProcessRangingData(peersData);
}

void
UwbDevice::OnUwbNotification(UwbNotificationData uwbNotificationData)
{
    // Handle the notification in a fire-and-forget fashion. This may change
    // later. Since std::async returns a future, and the future's
    // destructor waits for it to complete, we cannot just ignore the
    // returned future. To work around this, we move the returned future
    // into a shared_ptr, then pass this by value to the std::async's
    // lambda, increasing its reference count. This will ensure the future
    // is automatically destructed once the async lambda has completed.
    auto notificationHandlerFuture = std::make_shared<std::future<void>>();
    *notificationHandlerFuture = std::async(std::launch::async, [this, notificationHandlerFuture, uwbNotificationData = std::move(uwbNotificationData)]() {
        HandleUwbNotification(std::move(uwbNotificationData));
    });
}

void
UwbDevice::HandleUwbNotification(UwbNotificationData uwbNotificationData)
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

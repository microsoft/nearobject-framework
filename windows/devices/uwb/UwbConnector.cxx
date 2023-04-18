
#include <exception>
#include <ios>
#include <ranges>
#include <stdexcept>
#include <typeinfo>

#include <plog/Log.h>
#include <wil/result.h>

#include <uwb/UwbPeer.hxx>
#include <uwb/protocols/fira/UwbException.hxx>
#include <windows/devices/DeviceHandle.hxx>
#include <windows/devices/uwb/UwbConnector.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>

using namespace windows::devices;
using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

namespace uwb
{
struct RegisteredCallbackToken
{
    /**
     * @brief Construct a new Registered Callback Token object
     *
     * @param deregister the lambda that is passed by copy that handles deregistration.
     * The lambda assumes that the caller is holding the mutex UwbConnector::m_eventCallbacksGate
     */
    RegisteredCallbackToken(std::function<void(RegisteredCallbackToken*)> deregister) :
        Deregister([this, deregister = std::move(deregister)]() {
            return deregister(this);
        })
    {}
    virtual ~RegisteredCallbackToken() = default;

    /**
     * @brief Handles Deregistration
     * The lambda assumes that the caller is holding the mutex UwbConnector::m_eventCallbacksGate
     */
    std::function<void()> Deregister;
};

struct RegisteredSessionCallbackToken : public RegisteredCallbackToken
{
    RegisteredSessionCallbackToken(std::function<void(RegisteredCallbackToken*)> deregister, uint32_t sessionId) :
        RegisteredCallbackToken(std::move(deregister)), SessionId(sessionId){};
    virtual ~RegisteredSessionCallbackToken() = default;
    uint32_t SessionId;
};

struct OnSessionEndedToken : public RegisteredSessionCallbackToken
{
    OnSessionEndedToken(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionEnded> callback, std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredSessionCallbackToken(std::move(deregister), sessionId),
        Callback(std::move(callback)){};
    std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionEnded> Callback;
};
struct OnRangingStartedToken : public RegisteredSessionCallbackToken
{
    OnRangingStartedToken(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnRangingStarted> callback, std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredSessionCallbackToken(std::move(deregister), sessionId),
        Callback(std::move(callback)){};
    std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnRangingStarted> Callback;
};
struct OnRangingStoppedToken : public RegisteredSessionCallbackToken
{
    OnRangingStoppedToken(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnRangingStopped> callback, std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredSessionCallbackToken(std::move(deregister), sessionId),
        Callback(std::move(callback)){};
    std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnRangingStopped> Callback;
};
struct OnPeerPropertiesChangedToken : public RegisteredSessionCallbackToken
{
    OnPeerPropertiesChangedToken(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnPeerPropertiesChanged> callback, std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredSessionCallbackToken(std::move(deregister), sessionId),
        Callback(std::move(callback)){};
    std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnPeerPropertiesChanged> Callback;
};
struct OnSessionMembershipChangedToken : public RegisteredSessionCallbackToken
{
    OnSessionMembershipChangedToken(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionMembershipChanged> callback, std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredSessionCallbackToken(std::move(deregister), sessionId),
        Callback(std::move(callback)){};
    std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionMembershipChanged> Callback;
};

struct RegisteredDeviceCallbackToken : public RegisteredCallbackToken
{
    RegisteredDeviceCallbackToken(std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredCallbackToken(std::move(deregister)){};
    virtual ~RegisteredDeviceCallbackToken() = default;
};
struct OnStatusChangedToken : public RegisteredDeviceCallbackToken
{
    OnStatusChangedToken(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnStatusChanged> callback, std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredDeviceCallbackToken(std::move(deregister)),
        Callback(std::move(callback)){};
    std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnStatusChanged> Callback;
};
struct OnDeviceStatusChangedToken : public RegisteredDeviceCallbackToken
{
    OnDeviceStatusChangedToken(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnDeviceStatusChanged> callback, std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredDeviceCallbackToken(std::move(deregister)),
        Callback(std::move(callback)){};
    std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnDeviceStatusChanged> Callback;
};
struct OnSessionStatusChangedToken : public RegisteredDeviceCallbackToken
{
    OnSessionStatusChangedToken(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnSessionStatusChanged> callback, std::function<void(RegisteredCallbackToken*)> deregister) :
        RegisteredDeviceCallbackToken(std::move(deregister)),
        Callback(std::move(callback)){};
    std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnSessionStatusChanged> Callback;
};
} // namespace uwb

UwbConnector::UwbConnector(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{
}

UwbConnector::~UwbConnector()
{
    NotificationListenerStop();
}

const std::string&
UwbConnector::DeviceName() const noexcept
{
    return m_deviceName;
}

std::future<void>
UwbConnector::Reset()
{
    std::promise<void> resultPromise{};
    auto resultFuture = resultPromise.get_future();

    wil::unique_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    const UWB_DEVICE_RESET deviceReset{
        .size = sizeof(UWB_DEVICE_RESET),
        .resetConfig = UWB_RESET_CONFIG_UWBS_RESET,
    };
    UWB_STATUS status;

    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_DEVICE_RESET, const_cast<UWB_DEVICE_RESET*>(&deviceReset), sizeof deviceReset, &status, sizeof status, nullptr, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_DEVICE_RESET, hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Failed)));
        return resultFuture;
    } else {
        PLOG_DEBUG << "IOCTL_UWB_DEVICE_RESET succeeded";
        auto uwbStatus = UwbCxDdi::To(status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            resultPromise.set_value();
        }
    }

    return resultFuture;
}

std::future<UwbDeviceInformation>
UwbConnector::GetDeviceInformation()
{
    std::promise<UwbDeviceInformation> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::unique_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    std::vector<uint8_t> deviceInformationBuffer{};
    DWORD bytesRequired = offsetof(UWB_DEVICE_INFO, vendorSpecificInfo);

    // Attempt at most twice to obtain device information. On the first attempt,
    // we guess that no vendor-specific information will be supplied. If this is
    // the case, the first attempt will succeed. Otherwise, the buffer is grown
    // to account for the vendor specific information, and the IOCTL attempted a
    // second time.
    for (const auto i : std::ranges::iota_view{ 0, 2 }) {
        deviceInformationBuffer.resize(bytesRequired);
        PLOG_DEBUG << "IOCTL_UWB_GET_DEVICE_INFO attempt #" << (i + 1) << " with " << std::size(deviceInformationBuffer) << "-byte buffer";
        BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_DEVICE_INFO, nullptr, 0, std::data(deviceInformationBuffer), std::size(deviceInformationBuffer), &bytesRequired, nullptr);
        if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
            DWORD lastError = GetLastError();
            // Treat all errors other than insufficient buffer size as fatal.
            if (lastError != ERROR_MORE_DATA) {
                HRESULT hr = HRESULT_FROM_WIN32(lastError);
                PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_INFO, hr=" << std::showbase << std::hex << hr;
                resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Failed)));
                break;
            }
            // Attempt to retry the ioctl with the appropriate buffer size, which is now held in bytesRequired.
            const auto& deviceInformationPartial = *reinterpret_cast<UWB_DEVICE_INFO*>(std::data(deviceInformationBuffer));
            bytesRequired = deviceInformationPartial.size;
            continue;
        } else {
            PLOG_DEBUG << "IOCTL_UWB_GET_DEVICE_INFO succeeded";
            auto& deviceInformation = *reinterpret_cast<UWB_DEVICE_INFO*>(std::data(deviceInformationBuffer));
            auto uwbStatus = UwbCxDdi::To(deviceInformation.status);
            if (!IsUwbStatusOk(uwbStatus)) {
                resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
            } else {
                auto uwbDeviceInformation = UwbCxDdi::To(deviceInformation);
                resultPromise.set_value(std::move(uwbDeviceInformation));
            }
            break;
        }
    }

    return resultFuture;
}

std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>>
UwbConnector::GetCapabilities()
{
    std::promise<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>> resultPromise{};
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << hr;
        return resultFuture;
    }

    std::vector<uint8_t> uwbDeviceCapabilitiesBuffer{};
    DWORD bytesRequired = offsetof(UWB_DEVICE_CAPABILITIES, capabilityParams);

    for (const auto i : std::ranges::iota_view{ 0, 2 }) {
        uwbDeviceCapabilitiesBuffer.resize(bytesRequired);
        PLOG_DEBUG << "IOCTL_UWB_GET_DEVICE_CAPABILITIES attempt #" << (i + 1) << " with " << std::size(uwbDeviceCapabilitiesBuffer) << "-byte buffer";
        BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, std::data(uwbDeviceCapabilitiesBuffer), std::size(uwbDeviceCapabilitiesBuffer), &bytesRequired, nullptr);
        if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
            DWORD lastError = GetLastError();
            // Treat all errors other than insufficient buffer size as fatal.
            if (lastError != ERROR_MORE_DATA) {
                HRESULT hr = HRESULT_FROM_WIN32(lastError);
                PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_CAPABILITIES, hr=" << std::showbase << std::hex << hr;
                resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Failed)));
                break;
            }
            // Attempt to retry the ioctl with the appropriate buffer size, which is now held in bytesRequired.
            const auto& deviceCapabilitiesPartial = *reinterpret_cast<UWB_DEVICE_CAPABILITIES*>(std::data(uwbDeviceCapabilitiesBuffer));
            bytesRequired = deviceCapabilitiesPartial.size;
            continue;
        } else {
            PLOG_DEBUG << "IOCTL_UWB_GET_DEVICE_CAPABILITIES succeeded";
            auto& deviceCapabilities = *reinterpret_cast<UWB_DEVICE_CAPABILITIES*>(std::data(uwbDeviceCapabilitiesBuffer));
            auto uwbStatus = UwbCxDdi::To(deviceCapabilities.status);
            if (!IsUwbStatusOk(uwbStatus)) {
                resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
            } else {
                auto uwbDeviceCapabilities = UwbCxDdi::To(deviceCapabilities);
                resultPromise.set_value(std::make_tuple(uwbStatus, std::move(uwbDeviceCapabilities)));
            }
            break;
        }
    }

    return resultFuture;
}

std::future<std::tuple<::uwb::protocol::fira::UwbStatus, uint32_t>>
UwbConnector::GetSessionCount()
{
    std::promise<std::tuple<::uwb::protocol::fira::UwbStatus, uint32_t>> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    UWB_GET_SESSION_COUNT getSessionCount;

    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_SESSION_COUNT, nullptr, 0, &getSessionCount, sizeof getSessionCount, nullptr, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_SESSION_COUNT"
                   << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    } else {
        PLOG_DEBUG << "IOCTL_UWB_GET_SESSION_COUNT succeeded";
        auto uwbStatus = UwbCxDdi::To(getSessionCount.status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            resultPromise.set_value(std::move(std::make_tuple(uwbStatus, getSessionCount.sessionCount)));
        }
    }

    return resultFuture;
}

std::future<UwbStatus>
UwbConnector::SessionInitialize(uint32_t sessionId, UwbSessionType sessionType)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    const UWB_SESSION_INIT sessionInit{
        .size = sizeof(UWB_SESSION_INIT),
        .sessionId = sessionId,
        .sessionType = UwbCxDdi::From(sessionType),
    };
    UWB_STATUS status;

    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_SESSION_INIT, const_cast<UWB_SESSION_INIT*>(&sessionInit), sizeof sessionInit, &status, sizeof status, nullptr, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_SESSION_INIT for session id " << sessionId << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    } else {
        PLOG_DEBUG << "IOCTL_UWB_SESSION_INIT succeeded";
        auto uwbStatus = UwbCxDdi::To(status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            resultPromise.set_value(std::move(uwbStatus));
        }
    }

    return resultFuture;
}

std::future<UwbStatus>
UwbConnector::SessionDeinitialize(uint32_t sessionId)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    const UWB_SESSION_DEINIT sessionDeinit{
        .size = sizeof(UWB_SESSION_DEINIT),
        .sessionId = sessionId,
    };
    UWB_STATUS status;

    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_SESSION_DEINIT, const_cast<UWB_SESSION_DEINIT*>(&sessionDeinit), sizeof sessionDeinit, &status, sizeof status, nullptr, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_SESSION_DEINIT for session id " << sessionId << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    } else {
        PLOG_DEBUG << "IOCTL_UWB_SESSION_DEINIT succeeded";
        auto uwbStatus = UwbCxDdi::To(status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            resultPromise.set_value(std::move(uwbStatus));
        }
    }

    return resultFuture;
}

std::future<std::tuple<UwbStatus, UwbSessionState>>
UwbConnector::SessionGetState(uint32_t sessionId)
{
    std::promise<std::tuple<UwbStatus, UwbSessionState>> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    const UWB_GET_SESSION_STATE getSessionState{
        .size = sizeof(UWB_GET_SESSION_STATE),
        .sessionId = sessionId
    };
    UWB_SESSION_STATE_STATUS sessionStateStatus;

    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_SESSION_STATE, const_cast<UWB_GET_SESSION_STATE*>(&getSessionState), sizeof getSessionState, &sessionStateStatus, sizeof sessionStateStatus, nullptr, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_SESSION_STATE for session id " << sessionId
                   << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    } else {
        PLOG_DEBUG << "IOCTL_UWB_GET_SESSION_STATE succeeded";
        auto uwbStatus = UwbCxDdi::To(sessionStateStatus.status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            resultPromise.set_value(std::move(std::make_tuple(uwbStatus, UwbCxDdi::To(sessionStateStatus.sessionState))));
        }
    }

    return resultFuture;
}

std::future<UwbStatus>
UwbConnector::SessionRangingStart(uint32_t sessionId)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    const UWB_START_RANGING_SESSION startRanging{
        .size = sizeof(UWB_START_RANGING_SESSION),
        .sessionId = sessionId,
    };
    UWB_STATUS status;

    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_START_RANGING_SESSION, const_cast<UWB_START_RANGING_SESSION*>(&startRanging), sizeof startRanging, &status, sizeof status, nullptr, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_START_RANGING_SESSION for session id " << sessionId << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    } else {
        PLOG_DEBUG << "IOCTL_UWB_START_RANGING_SESSION succeeded";
        auto uwbStatus = UwbCxDdi::To(status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            resultPromise.set_value(std::move(uwbStatus));
        }
    }

    return resultFuture;
}

std::future<UwbStatus>
UwbConnector::SessionRangingStop(uint32_t sessionId)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    const UWB_STOP_RANGING_SESSION stopRanging{
        .size = sizeof(UWB_STOP_RANGING_SESSION),
        .sessionId = sessionId,
    };
    UWB_STATUS status;

    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_STOP_RANGING_SESSION, const_cast<UWB_STOP_RANGING_SESSION*>(&stopRanging), sizeof stopRanging, &status, sizeof status, nullptr, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_STOP_RANGING_SESSION for session id " << sessionId << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    } else {
        PLOG_DEBUG << "IOCTL_UWB_STOP_RANGING_SESSION succeeded";
        auto uwbStatus = UwbCxDdi::To(status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            resultPromise.set_value(std::move(uwbStatus));
        }
    }

    return resultFuture;
}

std::future<std::tuple<UwbStatus, std::optional<uint32_t>>>
UwbConnector::SessionGetRangingCount(uint32_t sessionId)
{
    std::promise<std::tuple<UwbStatus, std::optional<uint32_t>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_RANGING_COUNT

    return resultFuture;
}

std::future<UwbSessionUpdateMulicastListStatus>
UwbConnector::SessionUpdateControllerMulticastList(uint32_t sessionId, UwbMulticastAction multicastAction, std::vector<::uwb::UwbMacAddress> controlees)
{
    std::promise<UwbSessionUpdateMulicastListStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST

    return resultFuture;
}

std::future<std::tuple<UwbStatus, std::vector<UwbApplicationConfigurationParameter>>>
UwbConnector::GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes)
{
    std::promise<std::tuple<UwbStatus, std::vector<UwbApplicationConfigurationParameter>>> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::unique_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    UwbCxDdi::UwbGetApplicationConfigurationParameters uwbGetApplicationConfigurationParameters{
        .SessionId = sessionId,
        .ParameterTypes = std::move(applicationConfigurationParameterTypes),
    };
    auto getAppConfigParams = UwbCxDdi::From(uwbGetApplicationConfigurationParameters);
    auto getAppConfigParamsBuffer = std::data(getAppConfigParams);

    DWORD bytesRequired = offsetof(UWB_APP_CONFIG_PARAMS, appConfigParams);
    std::vector<uint8_t> getAppConfigParamsResultBuffer{};

    for (const auto i : std::ranges::iota_view{ 0, 2 }) {
        getAppConfigParamsResultBuffer.resize(bytesRequired);
        PLOG_DEBUG << "IOCTL_UWB_GET_APP_CONFIG_PARAMS attempt #" << (i + 1) << " with " << std::size(getAppConfigParamsResultBuffer) << "-byte buffer";
        BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_APP_CONFIG_PARAMS, std::data(getAppConfigParamsBuffer), std::size(getAppConfigParamsBuffer), std::data(getAppConfigParamsResultBuffer), std::size(getAppConfigParamsResultBuffer), &bytesRequired, nullptr);
        if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
            DWORD lastError = GetLastError();
            // Treat all errors other than insufficient buffer size as fatal.
            if (lastError != ERROR_MORE_DATA) {
                HRESULT hr = HRESULT_FROM_WIN32(lastError);
                PLOG_ERROR << "error when sending IOCTL_UWB_GET_APP_CONFIG_PARAMS, hr=" << std::showbase << std::hex << hr;
                resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Failed)));
                break;
            }
            // Attempt to retry the ioctl with the appropriate buffer size, which is now held in bytesRequired.
            const auto& appConfigParamsPartial = *reinterpret_cast<UWB_APP_CONFIG_PARAMS*>(std::data(getAppConfigParamsResultBuffer));
            bytesRequired = appConfigParamsPartial.size;
            continue;
        }

        PLOG_DEBUG << "IOCTL_UWB_GET_APP_CONFIG_PARAMS succeeded";
        auto& appConfigParamsResult = *reinterpret_cast<UWB_APP_CONFIG_PARAMS*>(std::data(getAppConfigParamsResultBuffer));
        auto uwbStatus = UwbCxDdi::To(appConfigParamsResult.status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            auto uwbAppConfigParams = UwbCxDdi::To(appConfigParamsResult);
            resultPromise.set_value(std::make_tuple(std::move(uwbStatus), std::move(uwbAppConfigParams)));
        }
        break; // for{1,2}
    }

    return resultFuture;
}

std::future<std::tuple<UwbStatus, std::vector<UwbSetApplicationConfigurationParameterStatus>>>
UwbConnector::SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameter> applicationConfigurationParameters)
{
    std::promise<std::tuple<UwbStatus, std::vector<UwbSetApplicationConfigurationParameterStatus>>> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    }

    UwbCxDdi::UwbSetApplicationConfigurationParameters uwbSetApplicationConfigurationParameters{
        .SessionId = sessionId,
        .Parameters = std::move(applicationConfigurationParameters),
    };

    auto paramsDdi = UwbCxDdi::From(uwbSetApplicationConfigurationParameters);
    auto paramsBuffer = std::data(paramsDdi);

    auto statusSize = offsetof(UWB_SET_APP_CONFIG_PARAMS_STATUS, appConfigParamsStatus[std::size(uwbSetApplicationConfigurationParameters.Parameters)]);
    std::vector<uint8_t> statusBuffer(statusSize);
    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_SET_APP_CONFIG_PARAMS, std::data(paramsBuffer), std::size(paramsBuffer), std::data(statusBuffer), statusSize, nullptr, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_SET_APP_CONFIG_PARAMS with sessionId " << sessionId << ", hr = " << std::showbase << std::hex << hr;
        resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
        return resultFuture;
    } else {
        PLOG_DEBUG << "IOCTL_UWB_SET_APP_CONFIG_PARAMS succeeded";
        auto& setApplicationConfigurationParametersStatus = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS_STATUS*>(std::data(statusBuffer));
        auto uwbStatus = UwbCxDdi::To(setApplicationConfigurationParametersStatus.status);
        if (!IsUwbStatusOk(uwbStatus)) {
            resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
        } else {
            auto uwbSetApplicationConfigurationParametersStatus = UwbCxDdi::To(setApplicationConfigurationParametersStatus);
            auto result = std::make_tuple(uwbSetApplicationConfigurationParametersStatus.Status, std::move(uwbSetApplicationConfigurationParametersStatus.ParameterStatuses));
            resultPromise.set_value(std::move(result));
        }
    }

    return resultFuture;
}

void
UwbConnector::HandleNotifications(std::stop_token stopToken)
{
    LOG_INFO << "uwb notification listener started for device " << DeviceName();

    auto handleDriver = m_notificationHandleDriver;

    while (!stopToken.stop_requested()) {
        DWORD bytesRequired = sizeof(UWB_NOTIFICATION_DATA);
        std::vector<uint8_t> uwbNotificationDataBuffer{};
        m_notificationOverlapped = {};
        for (const auto i : std::ranges::iota_view{ 0, 2 }) {
            uwbNotificationDataBuffer.resize(bytesRequired);
            PLOG_DEBUG << "IOCTL_UWB_NOTIFICATION attempt #" << (i + 1) << " with " << std::size(uwbNotificationDataBuffer) << "-byte buffer";
            BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, std::data(uwbNotificationDataBuffer), std::size(uwbNotificationDataBuffer), &bytesRequired, &m_notificationOverlapped);
            DWORD lastError = GetLastError();
            HRESULT hr = HRESULT_FROM_WIN32(lastError);
            PLOG_DEBUG << "IOCTL_UWB_NOTIFICATION attempt #" << (i + 1) << " with " << std::size(uwbNotificationDataBuffer) << "-byte buffer completed " << bytesRequired << " bytes required hr=" << std::showbase << std::hex << hr << " lastError " << std::showbase << std::hex << lastError;
            if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
                lastError = GetLastError();
                if (lastError == ERROR_IO_PENDING) {
                    // I/O has been pended, wait for it synchronously, but in an interruptable manner.
                    if (!LOG_IF_WIN32_BOOL_FALSE(GetOverlappedResult(handleDriver.get(), &m_notificationOverlapped, &bytesRequired, TRUE /* wait */))) {
                        lastError = GetLastError();
                        hr = HRESULT_FROM_WIN32(lastError);
                        if (lastError == ERROR_INSUFFICIENT_BUFFER || lastError == ERROR_MORE_DATA) {
                            // Driver indicated buffer was too small and put required size in 'bytesRequired'. Retry with new size.
                            const UWB_NOTIFICATION_DATA& notificationDataPartial = *reinterpret_cast<UWB_NOTIFICATION_DATA*>(std::data(uwbNotificationDataBuffer));
                            bytesRequired = notificationDataPartial.size;
                            LOG_VERBOSE << "IOCTL_UWB_NOTIFICATION insufficient buffer (hr=" << std::showbase << std::hex << hr << "), " << std::dec << bytesRequired << " bytes required, current size " << std::size(uwbNotificationDataBuffer) << " bytes";
                            continue;
                        } else if (lastError == ERROR_OPERATION_ABORTED) {
                            LOG_WARNING << "IOCTL_UWB_NOTIFICATION aborted";
                            break; // for({0,2})
                        } else {
                            PLOG_ERROR << "error when sending IOCTL_UWB_NOTIFICATION, hr=" << std::showbase << std::hex << hr;
                            break; // for({0,2})
                        }
                    }
                } else if (lastError == ERROR_MORE_DATA || lastError == ERROR_INSUFFICIENT_BUFFER) {
                    LOG_VERBOSE << "IOCTL_UWB_NOTIFICATION insufficient buffer, " << bytesRequired << " bytes required, current size " << std::size(uwbNotificationDataBuffer) << " bytes";
                    const UWB_NOTIFICATION_DATA& notificationDataPartial = *reinterpret_cast<UWB_NOTIFICATION_DATA*>(std::data(uwbNotificationDataBuffer));
                    bytesRequired = notificationDataPartial.size;
                    // Attempt to retry the ioctl with the appropriate buffer size, which is now held in bytesRequired.
                    continue;
                } else {
                    // Treat all other errors as fatal.
                    hr = HRESULT_FROM_WIN32(lastError);
                    PLOG_ERROR << "error when sending IOCTL_UWB_NOTIFICATION, hr=" << std::showbase << std::hex << hr;
                    break; // for({1,2})
                }
            }

            // Ensure some data was provided by the driver.
            if (uwbNotificationDataBuffer.empty()) {
                continue;
            }

            // Convert to neutral type and process the notification.
            const UWB_NOTIFICATION_DATA& notificationData = *reinterpret_cast<UWB_NOTIFICATION_DATA*>(std::data(uwbNotificationDataBuffer));
            auto uwbNotificationData = UwbCxDdi::To(notificationData);

            // Invoke callbacks with notification data.
            // Handle the notification in a fire-and-forget fashion. This may change
            // later. Since std::async returns a future, and the future's
            // destructor waits for it to complete, we cannot just ignore the
            // returned future. To work around this, we move the returned future
            // into a shared_ptr, then pass this by value to the std::async's
            // lambda, increasing its reference count. This will ensure the future
            // is automatically destructed once the async lambda has completed.
            auto notificationHandlerFuture = std::make_shared<std::future<void>>();
            *notificationHandlerFuture = std::async(std::launch::async, [this, notificationHandlerFuture, uwbNotificationData = std::move(uwbNotificationData)]() {
                DispatchCallbacks(std::move(uwbNotificationData));
            });
        }
    }

    LOG_INFO << "uwb notification listener stopped for device " << DeviceName();
}

/**
 * @brief Internal helper function to invoke the callbacks with the arg
 * If the weak_ptr callback held within a token is not valid, then the token is removed
 *
 * @tparam ArgTs the type of the args to be passed into the callbacks
 * @tparam TokenT the type of the token
 * @param tokens
 * @param args
 */
template <typename TokenT, typename... ArgTs>
void
InvokeCallbacks(std::vector<std::shared_ptr<TokenT>>& tokens, ArgTs&&... args)
{
    if (tokens.empty()) {
        PLOG_INFO << "Ignoring " << typeid(TokenT).name() << " event due to missing callbacks";
    }

    for (auto it = std::cbegin(tokens); it != std::cend(tokens);) {
        auto token = *it;
        auto callbackWeak = token->Callback;
        auto callbackShared = callbackWeak.lock();
        if (not callbackShared) {
            it = tokens.erase(it);
        } else {
            auto callback = *callbackShared;
            if (not callback) {
                it = tokens.erase(it);
            } else {
                auto remove = callback(std::forward<ArgTs>(args)...);
                if (remove) {
                    it = tokens.erase(it);
                } else {
                    it = std::next(it);
                }
            }
        }
    }
}

/**
 * @brief Internal helper function that does the same as InvokeCallbacks, but this time specifically for the session callback maps
 *
 * @tparam ArgTs the types of the arguments given to the callback
 * @tparam TokenT the type of the token
 * @param sessionMap
 * @param sessionId
 * @param arg
 */
template <typename TokenT, typename... ArgTs>
void
InvokeSessionCallbacks(std::unordered_map<uint32_t, std::vector<std::shared_ptr<TokenT>>>& sessionMap, uint32_t sessionId, ArgTs&&... args)
{
    auto node = sessionMap.extract(sessionId);
    if (node.empty()) {
        PLOG_INFO << "Ignoring " << typeid(TokenT).name() << " event due to missing callbacks";
        return;
    }
    auto& tokens = node.mapped();
    InvokeCallbacks(tokens, std::forward<ArgTs>(args)...);
    if (not tokens.empty()) {
        sessionMap.insert(std::move(node));
    }
}

void
UwbConnector::OnSessionEnded(uint32_t sessionId, ::uwb::UwbSessionEndReason sessionEndReason)
{
    PLOG_VERBOSE << "Session with id " << sessionId << " executing callbacks for session ended";
    InvokeSessionCallbacks(m_onSessionEndedCallbacks, sessionId, sessionEndReason);
}

void
UwbConnector::OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus statusMulticastList)
{
    uint32_t sessionId = statusMulticastList.SessionId;

    // TODO there's probably a way to create a range view like we did before so we don't actually loop through the peers before checking if there's any callbacks for this
    std::vector<::uwb::UwbPeer> peersAdded;
    for (const auto& peer : statusMulticastList.Status) {
        if (peer.Status == UwbStatusMulticast::OkUpdate) {
            peersAdded.push_back(::uwb::UwbPeer{ peer.ControleeMacAddress });
        }
    }

    std::vector<::uwb::UwbPeer> peersRemoved{};

    PLOG_VERBOSE << "Session with id " << statusMulticastList.SessionId << " executing callback for adding peers";

    InvokeSessionCallbacks(m_onSessionMembershipChangedCallbacks, sessionId, peersAdded, peersRemoved);

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
UwbConnector::OnSessionRangingData(::uwb::protocol::fira::UwbRangingData rangingData)
{
    uint32_t sessionId = rangingData.SessionId;

    PLOG_VERBOSE << "Session with id " << rangingData.SessionId << " processing new ranging data";

    // TODO there's probably a way to create a range view like we did before so we don't actually loop through the peers before checking if there's any callbacks for this
    std::vector<::uwb::UwbPeer> peersData;
    peersData.reserve(rangingData.RangingMeasurements.size());
    for (const auto& peerData : rangingData.RangingMeasurements) {
        ::uwb::UwbPeer data{ peerData };
        PLOG_VERBOSE << "Peer data: " << data.ToString();
        peersData.push_back(std::move(data));
    }

    InvokeSessionCallbacks(m_onPeerPropertiesChangedCallbacks, sessionId, peersData);
}

void
UwbConnector::DispatchCallbacks(::uwb::protocol::fira::UwbNotificationData uwbNotificationData)
{
    constexpr auto getStatusChangedCallback = [](auto&& callbacks) {
        return callbacks->OnStatusChanged;
    };
    constexpr auto getDeviceStatusChangedCallback = [](auto&& callbacks) {
        return callbacks->OnDeviceStatusChanged;
    };
    constexpr auto getSessionStatusChangedCallback = [](auto&& callbacks) {
        return callbacks->OnSessionStatusChanged;
    };

    LOG_DEBUG << "received notification: " << ToString(uwbNotificationData);

    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };

    std::visit([this](auto&& arg) {
        using ValueType = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<ValueType, UwbStatus>) {
            InvokeCallbacks(m_onStatusChangedCallbacks, arg);
        } else if constexpr (std::is_same_v<ValueType, UwbStatusDevice>) {
            InvokeCallbacks(m_onDeviceStatusChangedCallbacks, arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionStatus>) {
            InvokeCallbacks(m_onSessionStatusChangedCallbacks, arg);
            if (arg.State == UwbSessionState::Deinitialized) {
                OnSessionEnded(arg.SessionId, ::uwb::UwbSessionEndReason::Stopped);
            }
        } else if constexpr (std::is_same_v<ValueType, UwbSessionUpdateMulicastListStatus>) {
            OnSessionMulticastListStatus(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbRangingData>) {
            OnSessionRangingData(arg);
        }
    },
        uwbNotificationData);
}

void
UwbConnector::NotificationListenerStart()
{
    wil::shared_hfile notificationHandleDriver;
    const std::string notificationHandleDeviceName = m_deviceName + windows::drivers::uwbcx::UwbNotificationNamespace;
    auto hr = OpenDriverHandle(notificationHandleDriver, notificationHandleDeviceName.c_str(), true);
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << notificationHandleDeviceName << ", hr=" << hr;
        return;
    }

    m_notificationHandleDriver = std::move(notificationHandleDriver);
    m_notificationThread = std::jthread([this](std::stop_token stopToken) {
        HandleNotifications(std::move(stopToken));
    });
}

void
UwbConnector::NotificationListenerStop()
{
    LOG_IF_WIN32_BOOL_FALSE(CancelIoEx(m_notificationHandleDriver.get(), &m_notificationOverlapped));
    m_notificationThread.request_stop();
}

/**
 * @brief Internal helper function to try to deregister this token as this type
 *
 * @tparam T the type to try to dynamic_cast this token to
 * @param token
 * @param tokensMap
 * @return true if this succeeded in finding the class to cast the token to
 * @return false
 */
template <typename T>
bool
DeregisterSessionEventCallback(::uwb::RegisteredCallbackToken* token, std::unordered_map<uint32_t, std::vector<std::shared_ptr<T>>>& tokensMap)
{
    auto callback = dynamic_cast<T*>(token);
    if (not callback) {
        return false;
    }
    auto sessionId = callback->SessionId;

    auto node = tokensMap.extract(sessionId);
    if (node.empty()) {
        return true; // sessionId has no associated tokens
    }

    auto tokens = node.mapped();
    auto tokenIt = std::find_if(std::cbegin(tokens), std::cend(tokens), [callback](const auto& token) {
        return token.get() == callback;
    });
    if (tokenIt == std::cend(tokens)) {
        return true; // no associated token found, bail
    }
    tokens.erase(tokenIt);
    tokensMap.insert(std::move(node));
    return true;
}

/**
 * @brief Internal helper function to try to deregister this token as this type
 *
 * @tparam T the type to try to dynamic_cast this token to
 * @param token
 * @param tokens
 * @return true if this succeeded in finding the class to cast the token to
 * @return false
 */
template <typename T>
bool
DeregisterDeviceEventCallback(::uwb::RegisteredCallbackToken* token, std::vector<std::shared_ptr<T>>& tokens)
{
    auto callback = dynamic_cast<T*>(token);
    if (not callback) {
        return false;
    }

    auto tokenIt = std::find_if(std::cbegin(tokens), std::cend(tokens), [callback](const auto& token) {
        return token.get() == callback;
    });
    if (tokenIt == std::cend(tokens)) {
        return true; // no associated token found, bail
    }
    tokens.erase(tokenIt);
    return true;
}

/**
 * @brief Internal helper function that tokenizes a callback if it can be resolved
 *
 * @tparam LambdaT the type of the callback lambda
 * @param callbacks the struct holding the callbacks
 * @param accessor the accessor for the specific callback
 * @param tokenize the function that actually tokenizes the callback and gives ownership to the UwbConnector
 * @return std::weak_ptr<::uwb::RegisteredDeviceCallbackToken>
 */
template <typename LambdaT>
std::weak_ptr<::uwb::RegisteredDeviceCallbackToken>
GetToken(::uwb::UwbRegisteredDeviceEventCallbacks callbacks, std::function<std::weak_ptr<LambdaT>(::uwb::UwbRegisteredDeviceEventCallbacks)> accessor, std::function<std::weak_ptr<::uwb::RegisteredDeviceCallbackToken>(std::weak_ptr<LambdaT>)> tokenize)
{
    auto callbackWeak = accessor(callbacks);
    auto callbackShared = callbackWeak.lock();
    if (not callbackShared) {
        return std::shared_ptr<::uwb::RegisteredDeviceCallbackToken>();
    }
    auto callback = *callbackShared;
    if (not callback) {
        return std::shared_ptr<::uwb::RegisteredDeviceCallbackToken>();
    }
    return tokenize(callbackWeak);
}

::uwb::UwbRegisteredDeviceEventCallbackTokens
UwbConnector::RegisterDeviceEventCallbacks(::uwb::UwbRegisteredDeviceEventCallbacks callbacks)
{
    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };
    bool noCallbacksPrior = not CallbacksPresent();

    auto OnStatusChangedToken = GetToken<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnStatusChanged>(
        callbacks, [](auto&& callbackStruct) {
            return callbackStruct.OnStatusChanged;
        },
        [this](auto&& callback) {
            auto token = std::make_shared<::uwb::OnStatusChangedToken>(callback, [this](::uwb::RegisteredCallbackToken* token) {
                DeregisterDeviceEventCallback(token, m_onStatusChangedCallbacks);
            });
            m_onStatusChangedCallbacks.push_back(token);
            return token;
        });

    auto OnDeviceStatusChangedToken = GetToken<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnDeviceStatusChanged>(
        callbacks, [](auto&& callbackStruct) {
            return callbackStruct.OnDeviceStatusChanged;
        },
        [this](auto&& callback) {
            auto token = std::make_shared<::uwb::OnDeviceStatusChangedToken>(callback, [this](::uwb::RegisteredCallbackToken* token) {
                DeregisterDeviceEventCallback(token, m_onDeviceStatusChangedCallbacks);
            });
            m_onDeviceStatusChangedCallbacks.push_back(token);
            return token;
        });

    auto OnSessionStatusChangedToken = GetToken<::uwb::UwbRegisteredDeviceEventCallbackTypes::OnSessionStatusChanged>(
        callbacks, [](auto&& callbackStruct) {
            return callbackStruct.OnSessionStatusChanged;
        },
        [this](auto&& callback) {
            auto token = std::make_shared<::uwb::OnSessionStatusChangedToken>(callback, [this](::uwb::RegisteredCallbackToken* token) {
                DeregisterDeviceEventCallback(token, m_onSessionStatusChangedCallbacks);
            });
            m_onSessionStatusChangedCallbacks.push_back(token);
            return token;
        });

    if (noCallbacksPrior and CallbacksPresent()) {
        NotificationListenerStart();
    }

    return {
        OnStatusChangedToken,
        OnDeviceStatusChangedToken,
        OnSessionStatusChangedToken
    };
}

/**
 * @brief Internal helper function to insert a token into the map of session tokens
 *
 * @tparam T the type of the token
 * @param tokens the map of tokens to insert into
 * @param sessionId
 * @param token
 */
template <typename T>
void
InsertSessionToken(std::unordered_map<uint32_t, std::vector<std::shared_ptr<T>>>& tokens, uint32_t sessionId, std::shared_ptr<T> token)
{
    auto node = tokens.extract(sessionId);
    if (!node.empty()) {
        auto& callbacks = node.mapped();
        callbacks.push_back(token);
        tokens.insert(std::move(node));
    } else {
        tokens.insert({ sessionId, { token } });
    }
}

/**
 * @brief Internal helper function that tokenizes a callback if it can be resolved
 *
 * @tparam L the type of the callback lambda
 * @param sessionId
 * @param callbacks the struct holding the callbacks
 * @param accessor the accessor for the specific callback
 * @param tokenize the function that actually tokenizes the callback and gives ownership to the UwbConnector
 * @return std::weak_ptr<::uwb::RegisteredSessionCallbackToken>
 */
template <typename LambdaT>
std::weak_ptr<::uwb::RegisteredSessionCallbackToken>
GetToken(uint32_t sessionId, ::uwb::UwbRegisteredSessionEventCallbacks callbacks, std::function<std::weak_ptr<LambdaT>(::uwb::UwbRegisteredSessionEventCallbacks)> accessor, std::function<std::weak_ptr<::uwb::RegisteredSessionCallbackToken>(uint32_t, std::weak_ptr<LambdaT>)> tokenize)
{
    auto callbackWeak = accessor(callbacks);
    auto callbackShared = callbackWeak.lock();
    if (not callbackShared) {
        return std::shared_ptr<::uwb::RegisteredSessionCallbackToken>();
    }
    auto callback = *callbackShared;
    if (not callback) {
        return std::shared_ptr<::uwb::RegisteredSessionCallbackToken>();
    }
    return tokenize(sessionId, callbackWeak);
}

::uwb::UwbRegisteredSessionEventCallbackTokens
UwbConnector::RegisterSessionEventCallbacks(uint32_t sessionId, ::uwb::UwbRegisteredSessionEventCallbacks callbacks)
{
    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };
    bool noCallbacksPrior = not CallbacksPresent();

    auto OnSessionEndedToken = GetToken<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionEnded>(
        sessionId, callbacks, [](auto&& callbackStruct) {
            return callbackStruct.OnSessionEnded;
        },
        [this](uint32_t sessionId, auto&& callback) {
            auto token = std::make_shared<::uwb::OnSessionEndedToken>(sessionId, callback, [this](::uwb::RegisteredCallbackToken* token) {
                DeregisterSessionEventCallback(token, m_onSessionEndedCallbacks);
            });
            InsertSessionToken(m_onSessionEndedCallbacks, sessionId, token);
            return token;
        });

    auto OnRangingStartedToken = GetToken<::uwb::UwbRegisteredSessionEventCallbackTypes::OnRangingStarted>(
        sessionId, callbacks, [](auto&& callbackStruct) {
            return callbackStruct.OnRangingStarted;
        },
        [this](uint32_t sessionId, auto&& callback) {
            auto token = std::make_shared<::uwb::OnRangingStartedToken>(sessionId, callback, [this](::uwb::RegisteredCallbackToken* token) {
                DeregisterSessionEventCallback(token, m_onRangingStartedCallbacks);
            });
            InsertSessionToken(m_onRangingStartedCallbacks, sessionId, token);
            return token;
        });

    auto OnRangingStoppedToken = GetToken<::uwb::UwbRegisteredSessionEventCallbackTypes::OnRangingStopped>(
        sessionId, callbacks, [](auto&& callbackStruct) {
            return callbackStruct.OnRangingStopped;
        },
        [this](uint32_t sessionId, auto&& callback) {
            auto token = std::make_shared<::uwb::OnRangingStoppedToken>(sessionId, callback, [this](::uwb::RegisteredCallbackToken* token) {
                DeregisterSessionEventCallback(token, m_onRangingStoppedCallbacks);
            });
            InsertSessionToken(m_onRangingStoppedCallbacks, sessionId, token);
            return token;
        });

    auto OnPeerPropertiesChangedToken = GetToken<::uwb::UwbRegisteredSessionEventCallbackTypes::OnPeerPropertiesChanged>(
        sessionId, callbacks, [](auto&& callbackStruct) {
            return callbackStruct.OnPeerPropertiesChanged;
        },
        [this](uint32_t sessionId, auto&& callback) {
            auto token = std::make_shared<::uwb::OnPeerPropertiesChangedToken>(sessionId, callback, [this](::uwb::RegisteredCallbackToken* token) {
                DeregisterSessionEventCallback(token, m_onPeerPropertiesChangedCallbacks);
            });
            InsertSessionToken(m_onPeerPropertiesChangedCallbacks, sessionId, token);
            return token;
        });
    auto OnSessionMembershipChangedToken = GetToken<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionMembershipChanged>(
        sessionId, callbacks, [](auto&& callbackStruct) {
            return callbackStruct.OnSessionMembershipChanged;
        },
        [this](uint32_t sessionId, auto&& callback) {
            auto token = std::make_shared<::uwb::OnSessionMembershipChangedToken>(sessionId, callback, [this](::uwb::RegisteredCallbackToken* token) {
                DeregisterSessionEventCallback(token, m_onSessionMembershipChangedCallbacks);
            });
            InsertSessionToken(m_onSessionMembershipChangedCallbacks, sessionId, token);
            return token;
        });

    if (noCallbacksPrior and CallbacksPresent()) {
        NotificationListenerStart();
    }

    return {
        OnSessionEndedToken,
        OnRangingStartedToken,
        OnRangingStoppedToken,
        OnPeerPropertiesChangedToken,
        OnSessionMembershipChangedToken
    };
}

bool
UwbConnector::CallbacksPresent()
{
    return not(m_onSessionEndedCallbacks.empty() and m_onRangingStartedCallbacks.empty() and
        m_onRangingStoppedCallbacks.empty() and m_onPeerPropertiesChangedCallbacks.empty() and m_onSessionMembershipChangedCallbacks.empty() and
        m_onStatusChangedCallbacks.empty() and m_onDeviceStatusChangedCallbacks.empty() and m_onSessionStatusChangedCallbacks.empty());
}

void
UwbConnector::DeregisterEventCallback(std::weak_ptr<::uwb::RegisteredCallbackToken> token)
{
    auto tokenShared = token.lock();
    if (not tokenShared) {
        return;
    }
    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };

    auto callbacksPresentPrior = CallbacksPresent();

    tokenShared->Deregister();

    if ((not CallbacksPresent()) and callbacksPresentPrior) {
        NotificationListenerStop();
    }
}
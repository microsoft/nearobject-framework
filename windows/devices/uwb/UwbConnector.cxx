
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
    virtual ~RegisteredCallbackToken() = default;
};

struct RegisteredSessionCallbackToken : public RegisteredCallbackToken
{
    RegisteredSessionCallbackToken(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> callbacks) :
        SessionId(sessionId),
        Callbacks(std::move(callbacks)){};
    uint32_t SessionId;
    std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> Callbacks;
};

struct RegisteredDeviceCallbackToken : public RegisteredCallbackToken
{
    RegisteredDeviceCallbackToken(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callbacks) :
        Callbacks(Callbacks){};
    std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> Callbacks;
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

std::vector<std::shared_ptr<::uwb::UwbRegisteredDeviceEventCallbacks>>
UwbConnector::GetResolvedDeviceEventCallbacks()
{
    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };

    std::vector<std::shared_ptr<::uwb::UwbRegisteredDeviceEventCallbacks>> deviceEventCallbacks;
    deviceEventCallbacks.reserve(std::size(m_deviceEventCallbacks));

    for (auto it = std::begin(m_deviceEventCallbacks); it != std::end(m_deviceEventCallbacks);) {
        auto& deviceEventCallbackToken = *it;
        auto& deviceEventCallbackWeak = deviceEventCallbackToken->Callbacks;
        auto deviceEventCallback = deviceEventCallbackWeak.lock();
        if (deviceEventCallback != nullptr) {
            deviceEventCallbacks.push_back(std::move(deviceEventCallback));
            it = std::next(it);
        } else {
            it = m_deviceEventCallbacks.erase(it);
        }
    }

    return deviceEventCallbacks;
}

std::vector<std::shared_ptr<::uwb::UwbRegisteredSessionEventCallbacks>>
UwbConnector::GetResolvedSessionEventCallbacks(uint32_t sessionId)
{
    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };

    // Lookup the set of callbacks for this session id. If the node is empty, no
    // callbacks have ever been registered.
    auto node = m_sessionEventCallbacks.extract(sessionId);
    if (node.empty()) {
        return {};
    }

    // Get a reference to the existing list of callbacks and attempt to resolve
    // each one into a shared_ptr. If the weak pointer expired, remove it from
    // the vector, otherwise move the shared_ptr into the new container of them
    // to be returned to the caller.
    auto& tokens = node.mapped();

    std::vector<std::shared_ptr<::uwb::UwbRegisteredSessionEventCallbacks>> sessionEventCallbacks;
    for (auto it = std::begin(tokens); it != std::end(tokens);) {
        auto& sessionEventCallbackToken = *it;
        auto& sessionEventCallbackWeak = sessionEventCallbackToken->Callbacks;
        auto sessionEventCallback = sessionEventCallbackWeak.lock();
        if (sessionEventCallback != nullptr) {
            sessionEventCallbacks.push_back(std::move(sessionEventCallback));
            it = std::next(it);
        } else {
            // this callback is stale, erase it from the tokens
            it = tokens.erase(it);
        }
    }

    m_sessionEventCallbacks.insert(std::move(node));

    return sessionEventCallbacks;
}

void
UwbConnector::OnSessionEnded(uint32_t sessionId, ::uwb::UwbSessionEndReason sessionEndReason)
{
    auto sessionEventCallbacks = GetResolvedSessionEventCallbacks(sessionId);
    if (sessionEventCallbacks.empty()) {
        PLOG_DEBUG << "Ignoring SessionEnded event due to missing session callback";
        return;
    }

    PLOG_VERBOSE << "Session with id " << sessionId << " executing callbacks for session ended";

    constexpr auto hasOnSessionEnded = [](auto&& callback) {
        return callback->OnSessionEnded != nullptr;
    };
    std::ranges::for_each(sessionEventCallbacks | std::views::filter(hasOnSessionEnded), [&](auto&& callback) {
        callback->OnSessionEnded(sessionEndReason);
    });
}

void
UwbConnector::OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus statusMulticastList)
{
    uint32_t sessionId = statusMulticastList.SessionId;

    auto sessionEventCallbacks = GetResolvedSessionEventCallbacks(sessionId);
    if (sessionEventCallbacks.empty()) {
        PLOG_DEBUG << "Ignoring MulticastListStatus event due to missing session callback";
        return;
    }

    std::vector<::uwb::UwbPeer> peersAdded;
    for (const auto& peer : statusMulticastList.Status) {
        if (peer.Status == UwbStatusMulticast::OkUpdate) {
            peersAdded.push_back(::uwb::UwbPeer{ peer.ControleeMacAddress });
        }
    }

    PLOG_VERBOSE << "Session with id " << statusMulticastList.SessionId << " executing callback for adding peers";

    constexpr auto hasOnSessionMembershipChanged = [](auto&& callback) {
        return callback->OnSessionMembershipChanged != nullptr;
    };
    std::ranges::for_each(sessionEventCallbacks | std::views::filter(hasOnSessionMembershipChanged), [&](auto&& callback) {
        callback->OnSessionMembershipChanged(peersAdded, {});
    });

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

    auto sessionEventCallbacks = GetResolvedSessionEventCallbacks(sessionId);
    if (sessionEventCallbacks.empty()) {
        PLOG_VERBOSE << "Ignoring RangingData event due to missing session callback";
        return;
    }

    PLOG_VERBOSE << "Session with id " << rangingData.SessionId << " processing new ranging data";

    std::vector<::uwb::UwbPeer> peersData;
    peersData.reserve(rangingData.RangingMeasurements.size());
    for (const auto& peerData : rangingData.RangingMeasurements) {
        ::uwb::UwbPeer data{ peerData };
        PLOG_VERBOSE << "Peer data: " << data.ToString();
        peersData.push_back(std::move(data));
    }

    constexpr auto hasOnPeerPropertiesChanged = [](auto&& callback) {
        return callback->OnPeerPropertiesChanged != nullptr;
    };
    std::ranges::for_each(sessionEventCallbacks | std::views::filter(hasOnPeerPropertiesChanged), [&](auto&& callback) {
        callback->OnPeerPropertiesChanged(peersData);
    });
}

/**
 * @brief Helper function to handle the deregistration of missing callbacks
 *
 * @tparam ArgT the argument type of the specific callback
 * @param callbacks the structure holding the callbacks
 * @param callbackAccessor the lambda that returns the specific callback in
 * question. This function assumes that callbackAccessor(callbacks) is a valid
 * pointer.
 * @return bool True if the callback gets executed, False if the
 * callback needs to be deregistered
 */
template <typename ArgT>
bool
InvokeDeviceEventCallback(std::shared_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callbacks, std::function<std::function<void(ArgT)>(std::shared_ptr<::uwb::UwbRegisteredDeviceEventCallbacks>)> callbackAccessor, ArgT& arg)
{
    if (not callbacks) {
        PLOG_WARNING << "Ignoring " << typeid(ArgT).name() << " event due to missing callback";
        return false;
    }
    auto callback = callbackAccessor(callbacks);
    if (not callback) {
        PLOG_WARNING << "Ignoring " << typeid(ArgT).name() << " event due to missing callback";
        return false;
    }
    callback(arg);
    return true;
}

/**
 * @brief Wrapper for container-based invocation of InvokeDeviceEventCallback helper above.
 *
 * @tparam ArgT the argument type of the specific callback
 * @param callbacks the structure holding the callbacks
 * @param callbackAccessor the lambda that returns the specific callback in
 * question. This function assumes that callbackAccessor(callbacks) is a valid
 * pointer.
 */
template <typename ArgT>
void
InvokeDeviceEventCallbacks(std::vector<std::shared_ptr<::uwb::UwbRegisteredDeviceEventCallbacks>> callbacks, std::function<std::function<void(ArgT)>(std::shared_ptr<::uwb::UwbRegisteredDeviceEventCallbacks>)> callbackAccessor, ArgT& arg)
{
    std::ranges::for_each(callbacks, [&](auto&& callbacksInstance) {
        InvokeDeviceEventCallback(callbacksInstance, callbackAccessor, arg);
    });
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

    std::visit([this](auto&& arg) {
        using ValueType = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<ValueType, UwbStatus>) {
            auto deviceEventCallbacks = GetResolvedDeviceEventCallbacks();
            InvokeDeviceEventCallbacks<UwbStatus>(std::move(deviceEventCallbacks), getStatusChangedCallback, arg);
        } else if constexpr (std::is_same_v<ValueType, UwbStatusDevice>) {
            auto deviceEventCallbacks = GetResolvedDeviceEventCallbacks();
            InvokeDeviceEventCallbacks<UwbStatusDevice>(std::move(deviceEventCallbacks), getDeviceStatusChangedCallback, arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionStatus>) {
            auto deviceEventCallbacks = GetResolvedDeviceEventCallbacks();
            InvokeDeviceEventCallbacks<UwbSessionStatus>(std::move(deviceEventCallbacks), getSessionStatusChangedCallback, arg);
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
    auto hr = OpenDriverHandle(notificationHandleDriver, m_deviceName.c_str(), true);
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << hr;
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

std::weak_ptr<::uwb::RegisteredCallbackToken>
UwbConnector::RegisterDeviceEventCallbacks(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callbacks)
{
    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };
    bool isFirstCallback = not CallbacksPresent();
    auto token = InsertDeviceEventCallback(callbacks);
    if (isFirstCallback) {
        NotificationListenerStart();
    }
    return token;
}

std::weak_ptr<::uwb::RegisteredCallbackToken>
UwbConnector::RegisterSessionEventCallbacks(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> callbacks)
{
    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };
    bool isFirstCallback = not CallbacksPresent();
    auto token = InsertSessionEventCallback(sessionId, callbacks);
    if (isFirstCallback) {
        NotificationListenerStart();
    }
    return token;
}

bool
UwbConnector::CallbacksPresent()
{
    return (not m_sessionEventCallbacks.empty()) or (not m_deviceEventCallbacks.empty());
}

void
UwbConnector::DeregisterEventCallback(std::weak_ptr<::uwb::RegisteredCallbackToken> token)
{
    auto tokenShared = token.lock();
    if (not tokenShared) {
        return;
    }
    std::lock_guard eventCallbacksLockExclusive{ m_eventCallbacksGate };

    auto deviceCallback = std::dynamic_pointer_cast<::uwb::RegisteredDeviceCallbackToken>(tokenShared);
    if (deviceCallback) {
        // treat it as a device callback
        auto tokenIt = std::find_if(std::cbegin(m_deviceEventCallbacks), std::cend(m_deviceEventCallbacks), [deviceCallback](const auto& token) {
            return token.get() == deviceCallback.get();
        });
        if (tokenIt == std::cend(m_deviceEventCallbacks)) {
            return; // no associated token found, bail
        }
        m_deviceEventCallbacks.erase(tokenIt);
    } else {
        auto sessionCallback = dynamic_pointer_cast<::uwb::RegisteredSessionCallbackToken>(tokenShared);
        auto sessionId = sessionCallback->SessionId;

        auto node = m_sessionEventCallbacks.extract(sessionId);
        if (node.empty()) {
            return; // sessionId has no associated tokens
        }

        auto tokens = node.mapped();
        auto tokenIt = std::find_if(std::cbegin(tokens), std::cend(tokens), [sessionCallback](const auto& token) {
            return token.get() == sessionCallback.get();
        });
        if (tokenIt == std::cend(tokens)) {
            return; // no associated token found, bail
        }
        tokens.erase(tokenIt);
        m_sessionEventCallbacks.insert(std::move(node));
    }
}

std::shared_ptr<::uwb::RegisteredCallbackToken>
UwbConnector::InsertDeviceEventCallback(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callback)
{
    m_deviceEventCallbacks.push_back(std::make_shared<::uwb::RegisteredDeviceCallbackToken>(callback));
    return m_deviceEventCallbacks.back();
}

std::shared_ptr<::uwb::RegisteredCallbackToken>
UwbConnector::InsertSessionEventCallback(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> callback)
{
    // Obtain the map node to the existing vector of callbacks, if present.
    auto token = std::make_shared<::uwb::RegisteredSessionCallbackToken>(sessionId, callback);
    auto node = m_sessionEventCallbacks.extract(sessionId);
    if (!node.empty()) {
        auto& sessionEventCallbacks = node.mapped();
        sessionEventCallbacks.push_back(token);
        m_sessionEventCallbacks.insert(std::move(node));
    } else {
        m_sessionEventCallbacks.insert({ sessionId, { token } });
    }
    return token;
}

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
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDeviceConnector.hxx>

using namespace windows::devices;
using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

namespace uwb
{
class RegisteredCallbackToken
{
    uint32_t callbackId;
};
} // namespace uwb

UwbConnector::UwbConnector(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{
    NotificationListenerStart();
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
    DWORD bytesRequired = sizeof(UWB_DEVICE_INFO);

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

    // Determine the amount of memory required for the UWB_DEVICE_CAPABILITIES from the driver.
    DWORD bytesRequired = 0;
    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, nullptr, 0, &bytesRequired, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        // TODO: need to do something different here
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_CAPABILITIES, hr=" << std::showbase << std::hex << hr;
        return resultFuture;
    }

    // Allocate memory for the UWB_DEVICE_CAPABILITIES structure, and pass this to the driver request.
    DWORD uwbCapabilitiesSize = bytesRequired;
    auto uwbDeviceCapabilitiesBuffer = std::make_unique<uint8_t[]>(uwbCapabilitiesSize);
    ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, uwbDeviceCapabilitiesBuffer.get(), uwbCapabilitiesSize, &bytesRequired, nullptr);
    if (!ioResult) {
        // TODO: need to do something different here
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_CAPABILITIES, hr=" << std::showbase << std::hex << hr;
        return resultFuture;
    }

    const UWB_DEVICE_CAPABILITIES& uwbDeviceCapabilities = *reinterpret_cast<UWB_DEVICE_CAPABILITIES*>(uwbDeviceCapabilitiesBuffer.get());
    auto deviceCapabilities = UwbCxDdi::To(uwbDeviceCapabilities);
    auto uwbStatus = UwbCxDdi::To(uwbDeviceCapabilities.status);
    auto result = std::make_tuple(uwbStatus, std::move(deviceCapabilities));
    resultPromise.set_value(std::move(result));

    return resultFuture;
}

std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>>
UwbConnector::GetSessionCount()
{
    std::promise<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_SESSION_COUNT

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

std::future<std::tuple<UwbStatus, std::optional<UwbSessionState>>>
UwbConnector::SessionGetState(uint32_t sessionId)
{
    std::promise<std::tuple<UwbStatus, std::optional<UwbSessionState>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_SESSION_STATE

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

    DWORD bytesRequired = 0;
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
    DWORD bytesRequired = 0;
    std::vector<uint8_t> uwbNotificationDataBuffer{};
    auto handleDriver = m_notificationHandleDriver;

    while (!stopToken.stop_requested()) {
        m_notificationOverlapped = {};
        for (const auto i : std::ranges::iota_view{ 0, 2 }) {
            uwbNotificationDataBuffer.resize(bytesRequired);
            PLOG_DEBUG << "IOCTL_UWB_NOTIFICATION attempt #" << (i + 1) << " with " << std::size(uwbNotificationDataBuffer) << "-byte buffer";
            BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, std::data(uwbNotificationDataBuffer), std::size(uwbNotificationDataBuffer), &bytesRequired, &m_notificationOverlapped);
            if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
                DWORD lastError = GetLastError();
                if (lastError == ERROR_IO_PENDING) {
                    // I/O has been pended, wait for it synchronously
                    if (!LOG_IF_WIN32_BOOL_FALSE(GetOverlappedResult(handleDriver.get(), &m_notificationOverlapped, &bytesRequired, TRUE /* wait */))) {
                        lastError = GetLastError();
                        HRESULT hr = HRESULT_FROM_WIN32(lastError);
                        switch (lastError) {
                        case ERROR_INSUFFICIENT_BUFFER: // occurs for overlapped i/o
                        case ERROR_MORE_DATA:           // occurs for synchronous i/o
                            LOG_DEBUG << "IOCTL_UWB_NOTIFICATION completed, hr=" << std::showbase << std::hex << hr;
                            // driver indicated buffer was too small and put required size in 'bytesRequired'. Retry with new size.
                            continue;
                        case ERROR_OPERATION_ABORTED:
                            LOG_WARNING << "IOCTL_UWB_NOTIFICATION aborted";
                            break;
                        default:
                            PLOG_ERROR << "error when sending IOCTL_UWB_NOTIFICATION, hr=" << std::showbase << std::hex << hr;
                            break; // for({0,2})
                        }
                    }
                } else if (lastError == ERROR_MORE_DATA || lastError == ERROR_INSUFFICIENT_BUFFER) {
                    // Attempt to retry the ioctl with the appropriate buffer size, which is now held in bytesRequired.
                    continue;
                } else {
                    // Treat all other errors as fatal.
                    HRESULT hr = HRESULT_FROM_WIN32(lastError);
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
}

void
UwbConnector::OnSessionEnded(uint32_t sessionId, ::uwb::UwbSessionEndReason sessionEndReason)
{
    auto it = m_sessionEventCallbacks.find(sessionId);
    if (it == std::end(m_sessionEventCallbacks)) {
        PLOG_WARNING << "Ignoring SessionEnded event due to missing session callback";
        return;
    }

    auto& [_, callbacksWeak] = *it;
    auto callbacks = callbacksWeak.lock();
    if (callbacks->OnSessionEnded == nullptr) {
        PLOG_WARNING << "Ignoring SessionEnded event due to missing session callback";
        m_sessionEventCallbacks.erase(it);
        return;
    }

    PLOG_VERBOSE << "Session with id " << sessionId << " executing callback for session ended";
    callbacks->OnSessionEnded(sessionEndReason);
}

void
UwbConnector::OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus statusMulticastList)
{
    uint32_t sessionId = statusMulticastList.SessionId;
    auto it = m_sessionEventCallbacks.find(sessionId);
    if (it == std::end(m_sessionEventCallbacks)) {
        PLOG_WARNING << "Ignoring MulticastListStatus event due to missing session callback";
        return;
    }

    auto& [_, callbacksWeak] = *it;
    auto callbacks = callbacksWeak.lock();
    if (not(callbacks->OnSessionMembershipChanged)) {
        PLOG_WARNING << "Ignoring MulticastListStatus event due to missing session callback";
        m_sessionEventCallbacks.erase(it);
        return;
    }

    std::vector<::uwb::UwbPeer> peersAdded;
    for (const auto& peer : statusMulticastList.Status) {
        if (peer.Status == UwbStatusMulticast::OkUpdate) {
            peersAdded.push_back(::uwb::UwbPeer{ peer.ControleeMacAddress });
        }
    }

    PLOG_VERBOSE << "Session with id " << statusMulticastList.SessionId << " executing callback for adding peers";
    callbacks->OnSessionMembershipChanged(peersAdded, {});

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
    auto it = m_sessionEventCallbacks.find(sessionId);
    if (it == std::end(m_sessionEventCallbacks)) {
        PLOG_VERBOSE << "Ignoring RangingData event due to missing session callback";
        return;
    }

    auto& [_, callbacksWeak] = *it;
    auto callbacks = callbacksWeak.lock();
    if (not(callbacks->OnPeerPropertiesChanged)) {
        PLOG_WARNING << "Ignoring RangingData event due to expired session callback";
        m_sessionEventCallbacks.erase(it);
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

    callbacks->OnPeerPropertiesChanged(peersData);
}

/**
 * @brief Helper function to handle the deregistration of missing callbacks
 *
 * @tparam ArgT the argument type of the specific callback
 * @param callbacks the structure holding the callbacks
 * @param callbackAccessor the lambda that returns the specific callback in question. This function assumes that callbackAccessor(callbacks) is a valid pointer
 * @return bool True if the callback gets executed, False if the callback needs to be deregistered
 */
template <typename ArgT>
bool
Accessor(std::shared_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callbacks, std::function<std::function<void(ArgT)>(std::shared_ptr<::uwb::UwbRegisteredDeviceEventCallbacks>)> callbackAccessor, ArgT& arg)
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

void
UwbConnector::DispatchCallbacks(::uwb::protocol::fira::UwbNotificationData uwbNotificationData)
{
    std::shared_lock readerLock{ m_eventCallbacksGate };
    std::visit([this](auto&& arg) {
        using ValueType = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<ValueType, UwbStatus>) {
            auto callbacks = m_deviceEventCallbacks.lock();
            Accessor<UwbStatus>(
                callbacks, [](auto callbacks) {
                    return callbacks->OnStatusChanged;
                },
                arg);
        } else if constexpr (std::is_same_v<ValueType, UwbStatusDevice>) {
            auto callbacks = m_deviceEventCallbacks.lock();
            Accessor<UwbStatusDevice>(
                callbacks, [](auto callbacks) {
                    return callbacks->OnDeviceStatusChanged;
                },
                arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionStatus>) {
            auto callbacks = m_deviceEventCallbacks.lock();
            Accessor<UwbSessionStatus>(
                callbacks, [](auto callbacks) {
                    return callbacks->OnSessionStatusChanged;
                },
                arg);
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

bool
UwbConnector::NotificationListenerStart()
{
    std::unique_lock lock{ m_callbacksPresentConditionVariableGate };
    m_callbacksPresentConditionVariable.wait(lock, [this]() {
        return CallbacksPresent();
    });

    wil::shared_hfile notificationHandleDriver;
    auto hr = OpenDriverHandle(notificationHandleDriver, m_deviceName.c_str(), true);
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << hr;
        return false;
    }

    m_notificationHandleDriver = std::move(notificationHandleDriver);
    m_notificationThread = std::jthread([this](std::stop_token stopToken) {
        HandleNotifications(std::move(stopToken));
    });

    return true;
}

void
UwbConnector::NotificationListenerStop()
{
    LOG_IF_WIN32_BOOL_FALSE(CancelIoEx(m_notificationHandleDriver.get(), &m_notificationOverlapped));
    m_notificationThread.request_stop();
}

::uwb::RegisteredCallbackToken*
UwbConnector::RegisterDeviceEventCallbacks(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callbacks)
{
    {
        std::lock_guard presenceLock{ m_callbacksPresentConditionVariableGate };
        std::lock_guard writerLock{ m_eventCallbacksGate };
        m_deviceEventCallbacks = callbacks;
    }
    m_callbacksPresentConditionVariable.notify_one();
    return nullptr;
}

::uwb::RegisteredCallbackToken*
UwbConnector::RegisterSessionEventCallbacks(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> callbacks)
{
    {
        std::lock_guard presenceLock{ m_callbacksPresentConditionVariableGate };
        std::lock_guard writerLock{ m_eventCallbacksGate };
        m_sessionEventCallbacks.insert_or_assign(sessionId, callbacks);
    }
    m_callbacksPresentConditionVariable.notify_one();
    return nullptr;
}

bool
UwbConnector::CallbacksPresent()
{
    return m_deviceEventCallbacks.lock() and (not m_sessionEventCallbacks.empty());
}

void
UwbConnector::DeregisterEventCallback(::uwb::RegisteredCallbackToken* token)
{
    // TODO implement
    std::lock_guard writerLock{ m_eventCallbacksGate };
}
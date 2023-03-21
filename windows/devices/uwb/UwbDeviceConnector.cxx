
#include <exception>
#include <ios>
#include <ranges>
#include <stdexcept>

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

namespace windows::devices::uwb
{
class RegisteredCallbackToken
{
    uint32_t callbackId;
};
} // namespace windows::devices::uwb

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

UwbDeviceConnector::UwbDeviceConnector(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{}

UwbDeviceConnector::~UwbDeviceConnector()
{
    NotificationListenerStop();
}

const std::string&
UwbDeviceConnector::DeviceName() const noexcept
{
    return m_deviceName;
}

std::future<void>
UwbDeviceConnector::Reset()
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
UwbDeviceConnector::GetDeviceInformation()
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
    for (const auto i : std::ranges::iota_view{ 1, 2 }) {
        deviceInformationBuffer.resize(bytesRequired);
        PLOG_DEBUG << "IOCTL_UWB_GET_DEVICE_INFO attempt #" << i << " with " << std::size(deviceInformationBuffer) << "-byte buffer";
        BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_DEVICE_INFO, nullptr, 0, std::data(deviceInformationBuffer), std::size(deviceInformationBuffer), &bytesRequired, nullptr);
        if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
            DWORD lastError = GetLastError();
            // Treat all errors other than insufficient buffer size as fatal.
            if (lastError != ERROR_INSUFFICIENT_BUFFER) {
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
UwbDeviceConnector::GetCapabilities()
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
UwbDeviceConnector::GetSessionCount()
{
    std::promise<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_SESSION_COUNT

    return resultFuture;
}

std::future<UwbStatus>
UwbDeviceConnector::SessionInitialize(uint32_t sessionId, UwbSessionType sessionType)
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
UwbDeviceConnector::SessionDeinitialize(uint32_t sessionId)
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
UwbDeviceConnector::SessionGetState(uint32_t sessionId)
{
    std::promise<std::tuple<UwbStatus, std::optional<UwbSessionState>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_SESSION_STATE

    return resultFuture;
}

std::future<UwbStatus>
UwbDeviceConnector::SessionRangingStart(uint32_t sessionId)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_START_RANGING_SESSION

    return resultFuture;
}

std::future<UwbStatus>
UwbDeviceConnector::SessionRangingStop(uint32_t sessionId)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_STOP_RANGING_SESSION

    return resultFuture;
}

std::future<std::tuple<UwbStatus, std::optional<uint32_t>>>
UwbDeviceConnector::SessionGetRangingCount(uint32_t sessionId)
{
    std::promise<std::tuple<UwbStatus, std::optional<uint32_t>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_RANGING_COUNT

    return resultFuture;
}

std::future<UwbSessionUpdateMulicastListStatus>
UwbDeviceConnector::SessionUpdateControllerMulticastList(uint32_t sessionId, UwbMulticastAction multicastAction, std::vector<::uwb::UwbMacAddress> controlees)
{
    std::promise<UwbSessionUpdateMulicastListStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST

    return resultFuture;
}

std::future<std::tuple<UwbStatus, std::vector<UwbApplicationConfigurationParameter>>>
UwbDeviceConnector::GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes)
{
    std::promise<std::tuple<UwbStatus, std::vector<UwbApplicationConfigurationParameter>>> promiseResult;
    auto resultFuture = promiseResult.get_future();
    // TODO: invoke IOCTL_UWB_GET_APP_CONFIG_PARAMS

    return resultFuture;
}

std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<std::tuple<::uwb::protocol::fira::UwbApplicationConfigurationParameterType, ::uwb::protocol::fira::UwbStatus>>>>
UwbDeviceConnector::SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> applicationConfigurationParameters)
{
    std::promise<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<std::tuple<::uwb::protocol::fira::UwbApplicationConfigurationParameterType, ::uwb::protocol::fira::UwbStatus>>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_SET_APP_CONFIG_PARAMS

    //
    // Old code that used the original one-off UwbSetAppConfigurationParameters wrapper.
    // TODO: Once this is implemented, delete the commented out code.
    //

    // auto& setParamsBuffer = applicationConfigurationParameters.DdiBuffer();
    // auto& setParams = applicationConfigurationParameters.DdiParameters();

    // // Allocate memory for the UWB_SET_APP_CONFIG_PARAMS_STATUS structure, and pass this to the driver request
    // auto statusSize = offsetof(UWB_SET_APP_CONFIG_PARAMS_STATUS, appConfigParamsStatus[setParams.appConfigParamsCount]);
    // std::vector<uint8_t> statusBuffer(statusSize);
    // BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_SET_APP_CONFIG_PARAMS, std::data(setParamsBuffer), std::size(setParamsBuffer), std::data(statusBuffer), statusSize, nullptr, nullptr);
    // if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
    //     HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
    //     PLOG_ERROR << "error when sending IOCTL_UWB_SET_APP_CONFIG_PARAMS with sessionId " << sessionId << ", hr = " << std::showbase << std::hex << hr;
    //     resultPromise.set_exception(std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected)));
    //     return resultFuture;
    // } else {
    //     PLOG_DEBUG << "IOCTL_UWB_SET_APP_CONFIG_PARAMS succeeded";
    //     auto& uwbSetAppConfigParamsStatus = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS_STATUS*>(std::data(statusBuffer));
    //     auto uwbStatus = UwbCxDdi::To(uwbSetAppConfigParamsStatus.status);
    //     if (!IsUwbStatusOk(uwbStatus)) {
    //         resultPromise.set_exception(std::make_exception_ptr(UwbException(std::move(uwbStatus))));
    //     } else {
    //         // TODO: Could move this logic into its own UwbCxDdi::To() function
    //         std::vector<std::tuple<::uwb::protocol::fira::UwbApplicationConfigurationParameterType, ::uwb::protocol::fira::UwbStatus>> appConfigParamsStatusList;
    //         for (auto i = 0; i < uwbSetAppConfigParamsStatus.appConfigParamsCount; i++)
    //         {
    //             auto paramType = UwbCxDdi::To(uwbSetAppConfigParamsStatus.appConfigParamsStatus[i].paramType);
    //             auto paramStatus = UwbCxDdi::To(uwbSetAppConfigParamsStatus.appConfigParamsStatus[i].status);

    //             auto appConfigParamStatus = std::make_tuple(paramType, paramStatus);
    //             appConfigParamsStatusList.push_back(appConfigParamStatus);
    //         }
    //         auto result = std::make_tuple(uwbStatus, std::move(appConfigParamsStatusList));
    //         resultPromise.set_value(std::move(result));
    //     }
    // }

    return resultFuture;
}

void
UwbDeviceConnector::HandleNotifications(std::stop_token stopToken)
{
    DWORD bytesRequired = 0;
    std::vector<uint8_t> uwbNotificationDataBuffer{};
    auto handleDriver = m_notificationHandleDriver;

    while (!stopToken.stop_requested()) {
        m_notificationOverlapped = {};
        for (const auto i : std::ranges::iota_view{ 1, 2 }) {
            uwbNotificationDataBuffer.resize(bytesRequired);
            PLOG_DEBUG << "IOCTL_UWB_NOTIFICATION attempt #" << i << " with " << std::size(uwbNotificationDataBuffer) << "-byte buffer";
            BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, std::data(uwbNotificationDataBuffer), std::size(uwbNotificationDataBuffer), &bytesRequired, &m_notificationOverlapped);
            if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
                DWORD lastError = GetLastError();
                if (lastError == ERROR_IO_PENDING) {
                    // I/O has been pended, wait for it synchronously
                    if (!LOG_IF_WIN32_BOOL_FALSE(GetOverlappedResult(handleDriver.get(), &m_notificationOverlapped, &bytesRequired, TRUE /* wait */))) {
                        lastError = GetLastError();
                        HRESULT hr = HRESULT_FROM_WIN32(lastError);
                        PLOG(lastError == ERROR_OPERATION_ABORTED ? plog::debug : plog::error)
                            << "error waiting for IOCTL_UWB_NOTIFICATION completion, hr=" << std::showbase << std::hex << hr;
                        break; // for({1,2})
                    }
                } else if (lastError == ERROR_INSUFFICIENT_BUFFER) {
                    // Attempt to retry the ioctl with the appropriate buffer size, which is now held in bytesRequired.
                    continue;
                } else {
                    // Treat all other errors as fatal.
                    HRESULT hr = HRESULT_FROM_WIN32(lastError);
                    PLOG_ERROR << "error when sending IOCTL_UWB_NOTIFICATION, hr=" << std::showbase << std::hex << hr;
                    break; // for({1,2})
                }
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
UwbDeviceConnector::OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus statusMulticastList)
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
UwbDeviceConnector::OnSessionRangingData(::uwb::protocol::fira::UwbRangingData rangingData)
{
    uint32_t sessionId = rangingData.SessionId;
    auto it = m_sessionEventCallbacks.find(sessionId);
    if (it == std::end(m_sessionEventCallbacks)) {
        PLOG_WARNING << "Ignoring RangingData event due to missing session callback";
        return;
    }

    auto& [_, callbacksWeak] = *it;
    auto callbacks = callbacksWeak.lock();
    if (not(callbacks->OnPeerPropertiesChanged)) {
        PLOG_WARNING << "Ignoring RangingData event due to missing session callback";
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

#define ClassName(x) #x

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
        PLOG_WARNING << "Ignoring " << ClassName(ArgT) << " event due to missing callback";
        return false;
    }
    auto callback = callbackAccessor(callbacks);
    if (not callback) {
        PLOG_WARNING << "Ignoring " << ClassName(ArgT) << " event due to missing callback";
        return false;
    }
    callback(arg);
    return true;
}

void
UwbDeviceConnector::DispatchCallbacks(::uwb::protocol::fira::UwbNotificationData uwbNotificationData)
{
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
        } else if constexpr (std::is_same_v<ValueType, UwbSessionUpdateMulicastListStatus>) {
            OnSessionMulticastListStatus(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbRangingData>) {
            OnSessionRangingData(arg);
        }
    },
        uwbNotificationData);
}

bool
UwbDeviceConnector::NotificationListenerStart()
{
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
UwbDeviceConnector::NotificationListenerStop()
{
    LOG_IF_WIN32_BOOL_FALSE(CancelIoEx(m_notificationHandleDriver.get(), &m_notificationOverlapped));
    m_notificationThread.request_stop();
}

RegisteredCallbackToken*
UwbDeviceConnector::RegisterDeviceEventCallbacks(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callbacks)
{
    m_deviceEventCallbacks = callbacks;
    return nullptr;
}

RegisteredCallbackToken*
UwbDeviceConnector::RegisterSessionEventCallbacks(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> callbacks)
{
    m_sessionEventCallbacks.insert_or_assign(sessionId, callbacks);
    return nullptr;
}

void
UwbDeviceConnector::DeregisterEventCallback(RegisteredCallbackToken* token)
{
    // TODO implement
}
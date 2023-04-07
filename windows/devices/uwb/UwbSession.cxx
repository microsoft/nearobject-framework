
#include <algorithm>
#include <ios>
#include <memory>
#include <numeric>

#include <magic_enum.hpp>
#include <plog/Log.h>
#include <wil/result.h>

#include <uwb/protocols/fira/UwbException.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
#include <windows/devices/uwb/UwbDeviceConnector.hxx>
#include <windows/devices/uwb/UwbSession.hxx>

using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

UwbSession::UwbSession(uint32_t sessionId, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, UwbDevice *uwbDevice, ::uwb::protocol::fira::DeviceType deviceType) :
    ::uwb::UwbSession(sessionId, std::move(callbacks), deviceType)
{
    m_registeredCallbacks = std::make_shared<::uwb::UwbRegisteredSessionEventCallbacks>(
        [this](::uwb::UwbSessionEndReason reason) {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for UwbSessionEndReason, skipping";
                // TODO deregister
            }
            return callbacks->OnSessionEnded(this, reason);
        },
        [this]() {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for ranging started, skipping";
                // TODO deregister
            }
            return callbacks->OnRangingStarted(this);
        },
        [this]() {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for ranging stopped, skipping";
                // TODO deregister
            }
            return callbacks->OnRangingStopped(this);
        },
        [this](const std::vector<::uwb::UwbPeer> peersChanged) {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for ranging data, skipping";
                // TODO deregister
            }
            return callbacks->OnPeerPropertiesChanged(this, peersChanged);
        },
        [this](const std::vector<::uwb::UwbPeer> peersAdded, const std::vector<::uwb::UwbPeer> peersRemoved) {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for peer list changes, skipping";
                // TODO deregister
            }
            return callbacks->OnSessionMembershipChanged(this, peersAdded, peersRemoved);
        });

    auto uwbConnector = std::make_shared<UwbConnector>(uwbDevice->DeviceName());
    m_uwbDeviceConnector = uwbConnector;
    m_registeredCallbacksToken = m_uwbDeviceConnector->RegisterSessionEventCallbacks(m_sessionId, m_registeredCallbacks);
}

UwbSession::UwbSession(uint32_t sessionId, UwbDevice *uwbDevice, ::uwb::protocol::fira::DeviceType deviceType) :
    UwbSession(sessionId, std::weak_ptr<::uwb::UwbSessionEventCallbacks>{}, uwbDevice, deviceType)
{}

std::shared_ptr<IUwbSessionDdiConnector>
UwbSession::GetUwbDeviceConnector() noexcept
{
    return m_uwbDeviceConnector;
}

void
UwbSession::ConfigureImpl(const std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> configParams)
{
    PLOG_VERBOSE << "ConfigureImpl";

    UwbSessionType sessionType = UwbSessionType::RangingSession;

    // Request a new session from the driver.
    auto sessionInitResultFuture = m_uwbDeviceConnector->SessionInitialize(m_sessionId, sessionType);
    if (!sessionInitResultFuture.valid()) {
        PLOG_ERROR << "failed to initialize session";
        throw UwbException(UwbStatusGeneric::Rejected);
    }

    UwbStatus statusSessionInit = sessionInitResultFuture.get();
    if (!IsUwbStatusOk(statusSessionInit)) {
        LOG_ERROR << "failed to initialize session, " << ToString(statusSessionInit);
        throw UwbException(statusSessionInit);
    }

    // Set the application configuration parameters for the session.
    auto setAppConfigParamsFuture = m_uwbDeviceConnector->SetApplicationConfigurationParameters(m_sessionId, configParams);
    if (!setAppConfigParamsFuture.valid()) {
        PLOG_ERROR << "failed to set the application configuration parameters";
        throw UwbException(UwbStatusGeneric::Rejected);
    }

    auto [statusSetParameters, resultSetParameters] = setAppConfigParamsFuture.get();
    if (!IsUwbStatusOk(statusSetParameters)) {
        LOG_ERROR << "failed to set application configuration parameters, " << ToString(statusSetParameters);
        throw UwbException(statusSetParameters);
    }

    // Log an error for each of the parameters that were not successfuly set.
    // TODO: the caller probably wants to know about this, figure out the best way to signal the error.
    //       One option is to define a UwbSetApplicationConfigurationParameterException which has an
    //       accessor that just returns the vector entries with statusSetParameter != Ok
    for (const auto &[statusSetParameter, applicationConfigurationParameterType] : resultSetParameters) {
        if (!IsUwbStatusOk(statusSetParameter)) {
            LOG_ERROR << "failed to set application configuration parameter " << magic_enum::enum_name(applicationConfigurationParameterType) << ", status=" << ToString(statusSetParameter);
        }
    }
}

void
UwbSession::StartRangingImpl()
{
    // TODO: interface function should be modified to return a UwbStatus, and this value will be returned.
    UwbStatus status;
    uint32_t sessionId = GetId();
    auto resultFuture = m_uwbDeviceConnector->SessionRangingStart(sessionId);
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to start ranging for session id " << sessionId;
        status = UwbStatusGeneric::Failed;
        return; /* TODO: uwbStatus */
    }

    try {
        status = resultFuture.get();
    } catch (std::exception &e) {
        PLOG_ERROR << "caught exception attempting to start ranging for session id " << sessionId << "(" << e.what() << ")";
        status = UwbStatusGeneric::Failed;
    }

    // TODO: return uwbStatus;
}

void
UwbSession::StopRangingImpl()
{
    // TODO: interface function should be modified to return a UwbStatus, and this value will be returned.
    UwbStatus status;
    uint32_t sessionId = GetId();
    auto resultFuture = m_uwbDeviceConnector->SessionRangingStop(sessionId);
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to stop ranging for session id " << sessionId;
        status = UwbStatusGeneric::Failed;
        return; /* TODO: uwbStatus */
    }

    try {
        status = resultFuture.get();
    } catch (std::exception &e) {
        PLOG_ERROR << "caught exception attempting to stop ranging for session id " << sessionId << "(" << e.what() << ")";
        status = UwbStatusGeneric::Failed;
    }

    // TODO: return uwbStatus;
}

void
UwbSession::AddPeerImpl([[maybe_unused]] ::uwb::UwbMacAddress peerMacAddress)
{
    PLOG_VERBOSE << "AddPeerImpl";

    // TODO: convert code below to invoke IOCTL_UWB_SET_APP_CONFIG_PARAMS to use connector

    // TODO: two main options for updating the UWB-CLX peer list:
    //  1) Every time a peer is added (on-demand)
    //  2) Only when StartRanging() is called.
    // Below sample code exemplifies 1) for simplicity but this is not necessarily the way to go.
    //
    // TODO: request UWB-CLX to update controlee list per below pseudo-code,
    // which is *very* rough and some parts probably plain wrong:
    //

    // const auto macAddressLength = m_uwbMacAddressSelf.GetLength();
    // const auto macAddressessLength = macAddressLength * m_peers.size();

    // std::size_t appConfigParamsSize = 0;
    // appConfigParamsSize += macAddressessLength;
    // // TODO: all other memory required for this structure must be accounted for, the above calculation was left incomplete.
    // // Also, proper memory alignment of trailing structures in the allocated buffer has not been taken into account.
    // auto appConfigParamsBuffer = std::make_unique<uint8_t[]>(appConfigParamsSize);
    // auto *appConfigParams = reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS *>(appConfigParamsBuffer.get());
    // appConfigParams->sessionId = GetId();
    // appConfigParams->appConfigParamsCount = 2;
    // UWB_APP_CONFIG_PARAM *appConfigParamList = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(appConfigParams + 1);
    // UWB_APP_CONFIG_PARAM *appConfigParamNumberOfControlees = appConfigParamList + 0;
    // UWB_APP_CONFIG_PARAM *appConfigParamDstMacAddress = appConfigParamList + 1;

    // // Populate NUMBER_OF_CONTROLEES app configuration parameter.
    // auto &numberOfControleesPayload = *reinterpret_cast<uint8_t *>(appConfigParamNumberOfControlees + 1);
    // appConfigParamNumberOfControlees->paramType = UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES;
    // appConfigParamNumberOfControlees->paramLength = 1;
    // numberOfControleesPayload = static_cast<uint8_t>(m_peers.size());

    // // Populate DST_MAC_ADDRESS app configuration parameter.
    // auto dstMacAddressPayload = reinterpret_cast<uint8_t *>(appConfigParamDstMacAddress + 1);
    // appConfigParamDstMacAddress->paramType = UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS;
    // appConfigParamDstMacAddress->paramLength = static_cast<uint32_t>(macAddressessLength);
    // auto dstMacAddress = dstMacAddressPayload;
    // for (const auto &peer : m_peers) {
    //     const auto value = peer.GetValue();
    //     std::copy(std::cbegin(value), std::cend(value), dstMacAddress);
    //     std::advance(dstMacAddress, std::size(value));
    // }

    // // Attempt to set all new parameters.
    // DWORD bytesReturned = 0;
    // UWB_SET_APP_CONFIG_PARAMS_STATUS appConfigParamsStatus; // TODO: this needs to be dynamically allocated to fit returned content
    // BOOL ioResult = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SET_APP_CONFIG_PARAMS, &appConfigParams, static_cast<DWORD>(appConfigParamsSize), &appConfigParamsStatus, sizeof appConfigParamsStatus, &bytesReturned, nullptr);
    // if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
    //     // TODO
    //     HRESULT hr = GetLastError();
    //     PLOG_ERROR << "could not send params to driver, hr=" << std::showbase << std::hex << hr;
    // }
}

std::vector<UwbApplicationConfigurationParameter>
UwbSession::GetApplicationConfigurationParametersImpl()
{
    uint32_t sessionId = GetId();
    constexpr auto allParameterTypesArray = magic_enum::enum_values<UwbApplicationConfigurationParameterType>();
    std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes(std::cbegin(allParameterTypesArray), std::cend(allParameterTypesArray));

    auto resultFuture = m_uwbDeviceConnector->GetApplicationConfigurationParameters(sessionId, applicationConfigurationParameterTypes);
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to obtain application configuration parameters for session id " << sessionId;
        throw UwbException(UwbStatusGeneric::Failed);
    }

    try {
        auto [uwbStatus, applicationConfigurationParameters] = resultFuture.get();
        if (!IsUwbStatusOk(uwbStatus)) {
            throw UwbException(uwbStatus);
        }
        return applicationConfigurationParameters;
    } catch (UwbException &uwbException) {
        PLOG_ERROR << "caught exception attempting to obtain application configuration parameters for session id " << sessionId << " (" << ToString(uwbException.Status) << ")";
        throw uwbException;
    } catch (std::exception &e) {
        PLOG_ERROR << "caught unexpected exception attempting to obtain application configuration parameters for session id " << sessionId << " (" << e.what() << ")";
        throw e;
    }
}

UwbSessionState
UwbSession::GetSessionStateImpl()
{
    uint32_t sessionId = GetId();

    auto resultFuture = m_uwbDeviceConnector->SessionGetState(sessionId);
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to obtain session state for session id " << sessionId;
        throw UwbException(UwbStatusGeneric::Failed);
    }

    try {
        auto [uwbStatus, sessionState] = resultFuture.get();
        if (!IsUwbStatusOk(uwbStatus)) {
            // TODO: this value should be returned
        }
        return sessionState;
    } catch (UwbException &uwbException) {
        PLOG_ERROR << "caught exception attempting to obtain session state for session id " << sessionId << " (" << ToString(uwbException.Status) << ")";
        throw uwbException;
    } catch (std::exception &e) {
        PLOG_ERROR << "caught unexpected exception attempting to obtain session state for session id " << sessionId << " (" << e.what() << ")";
        throw e;
    }
}

void
UwbSession::DestroyImpl()
{
    uint32_t sessionId = GetId();
    auto resultFuture = m_uwbDeviceConnector->SessionDeinitialize(sessionId);
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to issue device deinitialization request for session id " << sessionId;
        throw UwbException(UwbStatusGeneric::Rejected);
    }

    try {
        resultFuture.get();
    } catch (UwbException &uwbException) {
        PLOG_ERROR << "caught exception attempting to to deinitialize for session id " << sessionId << " (" << ToString(uwbException.Status) << ")";
        throw uwbException;
    } catch (std::exception &e) {
        PLOG_ERROR << "caught unexpected exception attempting to deinitialize for session id " << sessionId << " (" << e.what() << ")";
        throw e;
    }
}

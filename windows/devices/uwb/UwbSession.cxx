
#include <algorithm>
#include <ios>
#include <memory>
#include <numeric>

#include <magic_enum.hpp>
#include <plog/Log.h>
#include <wil/result.h>

#include <uwb/protocols/fira/UwbException.hxx>
#include <uwb/protocols/fira/UwbOobConversions.hxx>
#include <windows/devices/uwb/UwbConnector.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
#include <windows/devices/uwb/UwbSession.hxx>

using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

UwbSession::UwbSession(uint32_t sessionId, std::weak_ptr<::uwb::UwbDevice> device, std::shared_ptr<IUwbSessionDdiConnector> uwbSessionConnector, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, ::uwb::protocol::fira::DeviceType deviceType) :
    ::uwb::UwbSession(sessionId, std::move(device), std::move(callbacks), deviceType),
    m_uwbSessionConnector(std::move(uwbSessionConnector))
{
    m_onSessionEndedCallback =
        std::make_shared<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionEnded>([this](::uwb::UwbSessionEndReason reason) {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for UwbSessionEndReason, skipping";
                return true;
            }

            ::uwb::UwbSession::OnSessionEnded(callbacks, reason);
            return false;
        });
    m_onSessionStatusChangedCallback =
        std::make_shared<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionStatusChanged>([this](::uwb::protocol::fira::UwbSessionState state, std::optional<::uwb::protocol::fira::UwbSessionReasonCode> reasonCode) {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for session status changed, skipping";
                return true;
            }

            ::uwb::UwbSession::OnSessionStateChanged(callbacks, state, reasonCode);
            return false;
        });
    m_onPeerPropertiesChangedCallback =
        std::make_shared<::uwb::UwbRegisteredSessionEventCallbackTypes::OnPeerPropertiesChanged>([this](std::vector<::uwb::UwbPeer> peersChanged) {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for ranging data, skipping";
                return true;
            }
            
            ::uwb::UwbSession::OnPeerPropertiesChanged(callbacks, std::move(peersChanged));
            return false;
        });
    m_onSessionMembershipChangedCallback =
        std::make_shared<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionMembershipChanged>([this](std::vector<::uwb::UwbPeer> peersAdded, std::vector<::uwb::UwbPeer> peersRemoved) {
            auto callbacks = ResolveEventCallbacks();
            if (callbacks == nullptr) {
                PLOG_WARNING << "missing session event callback for peer list changes, skipping";
                return true;
            }

            ::uwb::UwbSession::OnSessionMembershipChanged(callbacks, std::move(peersAdded), std::move(peersRemoved));
            return false;
        });

    m_registeredCallbacksTokens = m_uwbSessionConnector->RegisterSessionEventCallbacks(m_sessionId, { m_onSessionEndedCallback, m_onSessionStatusChangedCallback, m_onPeerPropertiesChangedCallback, m_onSessionMembershipChangedCallback });
}

UwbSession::UwbSession(uint32_t sessionId, std::weak_ptr<::uwb::UwbDevice> device, std::shared_ptr<IUwbSessionDdiConnector> uwbSessionConnector, ::uwb::protocol::fira::DeviceType deviceType) :
    UwbSession(sessionId, std::move(device), std::move(uwbSessionConnector), std::weak_ptr<::uwb::UwbSessionEventCallbacks>{}, deviceType)
{}

std::shared_ptr<IUwbSessionDdiConnector>
UwbSession::GetUwbSessionConnector() noexcept
{
    return m_uwbSessionConnector;
}

void
UwbSession::ConfigureImpl(const std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> configParams)
{
    PLOG_VERBOSE << "ConfigureImpl";

    UwbSessionType sessionType = UwbSessionType::RangingSession;

    // Request a new session from the driver.
    auto sessionInitResultFuture = m_uwbSessionConnector->SessionInitialize(m_sessionId, sessionType);
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
    auto setAppConfigParamsFuture = m_uwbSessionConnector->SetApplicationConfigurationParameters(m_sessionId, configParams);
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
    auto resultFuture = m_uwbSessionConnector->SessionRangingStart(sessionId);
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
    auto resultFuture = m_uwbSessionConnector->SessionRangingStop(sessionId);
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

UwbStatus
UwbSession::TryAddControleeImpl([[maybe_unused]] ::uwb::UwbMacAddress controleeMacAddress)
{
    PLOG_VERBOSE << "TryAddControleeImpl";
    if (!m_uwbSessionConnector) {
        PLOG_WARNING << "No associated connector";
        return UwbStatusGeneric::Failed;
    }

    auto params = GetApplicationConfigurationParameters({ UwbApplicationConfigurationParameterType::DestinationMacAddresses });
    if (std::size(params) != 1) {
        throw std::runtime_error("GetApplicationConfigurationParameters() for 1 parameter did not return exactly 1 result. This is a bug!");
    }
    auto macAddresses = std::get<std::unordered_set<::uwb::UwbMacAddress>>(params.front().Value);
    auto [_, inserted] = macAddresses.insert(controleeMacAddress);
    if (!inserted) {
        PLOG_INFO << "controleeMacAddress already added, skipping";
        return UwbStatusSession::AddressAlreadyPresent;
    }
    UwbApplicationConfigurationParameter dstMacAddresses{ .Type = UwbApplicationConfigurationParameterType::DestinationMacAddresses, .Value = macAddresses };
    UwbApplicationConfigurationParameter numControlees{ .Type = UwbApplicationConfigurationParameterType::NumberOfControlees, .Value = static_cast<uint8_t>(std::size(macAddresses)) };

    SetApplicationConfigurationParameters({ numControlees, dstMacAddresses });

    return UwbStatusGeneric::Ok;
}

std::vector<UwbApplicationConfigurationParameter>
UwbSession::GetApplicationConfigurationParametersImpl(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> requestedTypes)
{
    uint32_t sessionId = GetId();

    auto resultFuture = m_uwbSessionConnector->GetApplicationConfigurationParameters(sessionId, requestedTypes);
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

void
UwbSession::SetApplicationConfigurationParametersImpl(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> uwbApplicationConfigurationParameters)
{
    uint32_t sessionId = GetId();
    auto resultFuture = m_uwbSessionConnector->SetApplicationConfigurationParameters(sessionId, uwbApplicationConfigurationParameters);
    try {
        auto [uwbStatus, applicationConfigurationParametersStatus] = resultFuture.get();

        if (IsUwbStatusOk(uwbStatus)) {
            return;
        } else {
            throw UwbException(uwbStatus);
        }
    } catch (UwbException &uwbException) {
        PLOG_ERROR << "caught exception attempting to set application configuration parameters for session id " << sessionId << " (" << ToString(uwbException.Status) << ")";
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

    auto resultFuture = m_uwbSessionConnector->SessionGetState(sessionId);
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
    auto resultFuture = m_uwbSessionConnector->SessionDeinitialize(sessionId);
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

std::vector<uint8_t>
UwbSession::GetOobDataObjectImpl()
{
    auto applicationConfigurationParameters = GetApplicationConfigurationParameters(AllParameters);
    auto uwbSessionData = GetUwbSessionData(applicationConfigurationParameters);

    uwbSessionData.sessionId = GetId();
    auto dataObject = uwbSessionData.ToDataObject();

    return dataObject->ToBytes();
}

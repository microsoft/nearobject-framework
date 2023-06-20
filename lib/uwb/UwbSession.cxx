
#include <cstdint>
#include <stdexcept>

#include <magic_enum.hpp>
#include <plog/Log.h>

#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbException.hxx>

using namespace uwb;
using namespace uwb::protocol::fira;

/* static */
const std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> UwbSession::AllParameters = {};

UwbSession::UwbSession(uint32_t sessionId, std::weak_ptr<UwbDevice> device, std::weak_ptr<UwbSessionEventCallbacks> callbacks, DeviceType deviceType) :
    m_deviceType{ deviceType },
    m_sessionId(sessionId),
    m_uwbMacAddressSelf(UwbMacAddress::Random<UwbMacAddressType::Extended>()),
    m_callbacks(std::move(callbacks)),
    m_device(std::move(device))
{}

UwbSession::UwbSession(uint32_t sessionId, std::weak_ptr<UwbDevice> device, DeviceType deviceType) :
    UwbSession(sessionId, std::move(device), std::weak_ptr<UwbSessionEventCallbacks>{}, deviceType)
{}

std::weak_ptr<UwbSessionEventCallbacks>
UwbSession::GetEventCallbacks() noexcept
{
    std::shared_lock callbacksLockShared{ m_callbacksGate };
    return m_callbacks;
}

void
UwbSession::SetEventCallbacks(std::weak_ptr<UwbSessionEventCallbacks> callbacks) noexcept
{
    {
        std::unique_lock<std::shared_mutex> callbacksLockExclusive;
        m_callbacks.swap(callbacks);
    }

    if (callbacks.lock() != nullptr) {
        LOG_WARNING << "Session with id " << m_sessionId << " callbacks were replaced";
    }
}

std::shared_ptr<UwbSessionEventCallbacks>
UwbSession::ResolveEventCallbacks() noexcept
{
    std::shared_lock callbacksLockShared{ m_callbacksGate };
    return m_callbacks.lock();
}

uwb::protocol::fira::DeviceType
UwbSession::GetDeviceType() const noexcept
{
    return m_deviceType;
}

uint32_t
UwbSession::GetId() const noexcept
{
    return m_sessionId;
}

void
UwbSession::SetMacAddressType(UwbMacAddressType uwbMacAddressType) noexcept
{
    m_uwbMacAddressType = uwbMacAddressType;
    // TODO: update driver with new mac address type (aka "mode" in FiRa-speak).
}

UwbStatus
UwbSession::TryAddControlee(UwbMacAddress controleeMacAddress)
{
    std::scoped_lock peersLock{ m_peerGate };
    PLOG_VERBOSE << "Session with id " << m_sessionId << " requesting to add controlee with mac address " << controleeMacAddress.ToString();

    auto uwbStatus = TryAddControleeImpl(std::move(controleeMacAddress));
    return uwbStatus;
}

void
UwbSession::Configure(const std::vector<protocol::fira::UwbApplicationConfigurationParameter> configParams)
{
    PLOG_VERBOSE << "configure session with id " << m_sessionId;
    try {
        ConfigureImpl(configParams);
    } catch (UwbException& uwbException) {
        PLOG_ERROR << "error configuring session with id " << m_sessionId << ", status=" << ToString(uwbException.Status);
        throw uwbException;
    } catch (std::exception& e) {
        PLOG_ERROR << "error configuring session with id " << m_sessionId << ", unexpected exception status=" << e.what();
        throw e;
    }
}

void
UwbSession::StartRanging()
{
    PLOG_VERBOSE << "start ranging";
    bool rangingActiveExpected = false;
    const bool wasRangingInactive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, true);
    if (wasRangingInactive) {
        StartRangingImpl();
    }
}

void
UwbSession::StopRanging()
{
    PLOG_VERBOSE << "stop ranging";
    bool rangingActiveExpected = true;
    const bool wasRangingActive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, false);
    if (wasRangingActive) {
        StopRangingImpl();
    }
}

void
UwbSession::InsertPeerImpl(const uwb::UwbMacAddress& peerAddress)
{
    m_peers.insert(peerAddress);
    PLOG_VERBOSE << "Session with id " << m_sessionId << " added peer via DDI with mac address " << peerAddress.ToString();
}

std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
UwbSession::GetApplicationConfigurationParameters(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> requestedTypes)
{
    PLOG_VERBOSE << "get application configuration parameters";
    return GetApplicationConfigurationParametersImpl(std::move(requestedTypes));
}

void
UwbSession::SetApplicationConfigurationParameters(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> uwbApplicationConfigurationParameters)
{
    PLOG_VERBOSE << "set application configuration parameters";
    return SetApplicationConfigurationParametersImpl(std::move(uwbApplicationConfigurationParameters));
}

UwbSessionState
UwbSession::GetSessionState()
{
    PLOG_VERBOSE << "get session state";
    return GetSessionStateImpl();
}

void
UwbSession::Destroy()
{
    PLOG_VERBOSE << "destroy session with id " << m_sessionId;
    DestroyImpl();
}

std::vector<uint8_t>
UwbSession::GetOobDataObject()
{
    PLOG_VERBOSE << "get OOB data object";
    return GetOobDataObjectImpl();
}

void
UwbSession::OnSessionStateChanged(std::shared_ptr<uwb::UwbSessionEventCallbacks> callbacks, ::uwb::protocol::fira::UwbSessionState state, std::optional<::uwb::protocol::fira::UwbSessionReasonCode> reasonCode)
{
    // Obtain old/existing state, and update with new state.
    // Note: no lock is needed to update m_state since the event hanlder guarantees sequential execution.
    const auto stateOld = m_state;
    m_state = state;

    PLOG_VERBOSE << "Session with id " << m_sessionId << " state changed: " << magic_enum::enum_name(stateOld) << " --> " << magic_enum::enum_name(state);

    // Check if the session transitioned into the ranging state.
    if (state == UwbSessionState::Active) {
        bool rangingActiveExpected = false;
        if (m_rangingActive.compare_exchange_weak(rangingActiveExpected, true)) {
            callbacks->OnRangingStarted(this);
        }
    // Check if the session transitioned out of the ranging state.
    } else if (state == UwbSessionState::Idle) {
        bool rangingActiveExpected = true;
        if (m_rangingActive.compare_exchange_weak(rangingActiveExpected, false)) {
            callbacks->OnRangingStopped(this);
        }
    }
}

void
UwbSession::OnSessionEnded(std::shared_ptr<uwb::UwbSessionEventCallbacks> callbacks, ::uwb::UwbSessionEndReason reason)
{
    PLOG_VERBOSE << "Session with id " << m_sessionId << " ended";
    callbacks->OnSessionEnded(this, reason);
}

void
UwbSession::OnPeerPropertiesChanged(std::shared_ptr<uwb::UwbSessionEventCallbacks> callbacks, std::vector<::uwb::UwbPeer> peersChanged)
{
    PLOG_VERBOSE << "Session with id " << m_sessionId << " peer properties changed";
    callbacks->OnPeerPropertiesChanged(this, std::move(peersChanged));
}

void
UwbSession::OnSessionMembershipChanged(std::shared_ptr<uwb::UwbSessionEventCallbacks> callbacks, std::vector<::uwb::UwbPeer> peersAdded, std::vector<::uwb::UwbPeer> peersRemoved)
{
    PLOG_VERBOSE << "Session with id " << m_sessionId << " session membership changed";
    callbacks->OnSessionMembershipChanged(this, std::move(peersAdded), std::move(peersRemoved));
}

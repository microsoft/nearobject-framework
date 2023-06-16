
#include <cstdint>
#include <stdexcept>

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
UwbSession::SetSessionStatus(const uwb::protocol::fira::UwbSessionStatus& status)
{
    PLOG_VERBOSE << "session changed state: " << m_sessionStatus.ToString() << " --> " << status.ToString();
    m_sessionStatus = status;
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

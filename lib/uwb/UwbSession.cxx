
#include <cstdint>
#include <stdexcept>

#include <plog/Log.h>

#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbException.hxx>

using namespace uwb;
using namespace uwb::protocol::fira;

UwbSession::UwbSession(uint32_t sessionId, std::weak_ptr<UwbSessionEventCallbacks> callbacks, DeviceType deviceType) :
    m_uwbMacAddressSelf(UwbMacAddress::Random<UwbMacAddressType::Extended>()),
    m_callbacks(std::move(callbacks)),
    m_deviceType{ deviceType },
    m_sessionId(sessionId)
{}

UwbSession::UwbSession(uint32_t sessionId, DeviceType deviceType) :
    UwbSession(sessionId, std::weak_ptr<UwbSessionEventCallbacks>{}, deviceType)
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

void
UwbSession::AddPeer(UwbMacAddress peerMacAddress)
{
    std::scoped_lock peersLock{ m_peerGate };
    PLOG_VERBOSE << "Session with id " << m_sessionId << " requesting to add peer via DDI with mac address " << peerMacAddress.ToString();
    AddPeerImpl(std::move(peerMacAddress));
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
UwbSession::GetApplicationConfigurationParameters()
{
    PLOG_VERBOSE << "get application configuration parameters";
    return GetApplicationConfigurationParametersImpl();
}

void
UwbSession::Destroy()
{
    PLOG_VERBOSE << "destroy session with id " << m_sessionId;
    DestroyImpl();
}


#include <cstdint>
#include <stdexcept>

#include <plog/Log.h>

#include <uwb/protocols/fira/UwbException.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>

using namespace uwb;
using namespace uwb::protocol::fira;

UwbSession::UwbSession(std::weak_ptr<UwbSessionEventCallbacks> callbacks) :
    m_uwbMacAddressSelf(UwbMacAddress::Random<UwbMacAddressType::Extended>()),
    m_callbacks(std::move(callbacks))
{}

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
UwbSession::Configure(const uwb::protocol::fira::UwbSessionData& uwbSessionData)
{
    PLOG_VERBOSE << "configure with with id " << m_sessionId;
    try {
        ConfigureImpl(uwbSessionData);
    } catch (UwbException &uwbException) {
        PLOG_ERROR << "error configuring session with id " << m_sessionId << ", status=" << ToString(uwbException.Status);
        throw uwbException;
    } catch (std::exception &e) {
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


#include <cstdint>
#include <stdexcept>

#include <plog/Log.h>

#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>

using namespace uwb;

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
    PLOG_VERBOSE << "adding peer with address " << peerMacAddress.ToString();
    auto [_, inserted] = m_peers.insert(peerMacAddress);

    if (inserted) {
        AddPeerImpl(std::move(peerMacAddress));
    }
    PLOG_VERBOSE << "peer added";
}

void
UwbSession::Configure(const uwb::protocol::fira::UwbSessionData& uwbSessionData)
{
    PLOG_VERBOSE << "configure";
    ConfigureImpl(uwbSessionData);
}

void
UwbSession::StartRanging()
{
    PLOG_VERBOSE << "start ranging";
    bool rangingActiveExpected = false;
    const bool wasRangingActive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, true);
    if (wasRangingActive) {
        return;
    }

    StartRangingImpl();
}

void
UwbSession::StopRanging()
{
    PLOG_VERBOSE << "stop ranging";
    bool rangingActiveExpected = true;
    const bool wasRangingActive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, false);
    if (!wasRangingActive) {
        return;
    }
}

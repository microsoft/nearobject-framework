
#include <cstdint>
#include <stdexcept>

#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>

using namespace uwb;

UwbSession::UwbSession(uint32_t sessionId, std::weak_ptr<UwbSessionEventCallbacks> callbacks) :
    m_sessionId(sessionId),
    m_callbacks(std::move(callbacks)),
    m_uwbMacAddressSelf(UwbMacAddress::Random<UwbMacAddressType::Extended>())
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
    auto [_, inserted] = m_peers.insert(peerMacAddress);

    if (inserted) {
        AddPeerImpl(std::move(peerMacAddress));
    }
}

void
UwbSession::StartRanging()
{
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
    bool rangingActiveExpected = true;
    const bool wasRangingActive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, false);
    if (!wasRangingActive) {
        return;
    }
}

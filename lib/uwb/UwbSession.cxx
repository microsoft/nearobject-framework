
#include <stdexcept>
#include <uwb/UwbSession.hxx>

using namespace uwb;

namespace detail
{
UwbMacAddress
GenerateRandomUwbMacAddress(UwbMacAddressType uwbMacAddressType)
{
    // TODO: implement this correctly and put it somewhere shared

    switch (uwbMacAddressType) {
    case UwbMacAddressType::Short:
        return UwbMacAddress{ std::array<uint8_t, 2>{ 0x00, 0x01 } };
    case UwbMacAddressType::Extended:
        return UwbMacAddress{ std::array<uint8_t, 8>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 } };
    default:
        throw std::runtime_error("invalid mac address type specified");
    }
}
}

UwbSession::UwbSession(uint32_t sessionId) :
    m_sessionId(sessionId),
    m_uwbMacAddressSelf(::detail::GenerateRandomUwbMacAddress(UwbMacAddressType::Extended))
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

/**
 * @brief 
 */
void 
UwbSession::StopRanging()
{
    bool rangingActiveExpected = true;
    const bool wasRangingActive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, false);
    if (!wasRangingActive) {
        return;
    }
}

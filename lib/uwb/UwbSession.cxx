
#include <uwb/UwbSession.hxx>

using namespace uwb;

UwbSession::UwbSession(uint32_t sessionId, std::unordered_set<UwbMacAddress> peers) :
    m_sessionId(sessionId),
    m_peers(std::move(peers))
{}

uint32_t
UwbSession::GetId() const noexcept
{
    return m_sessionId;
}

void
UwbSession::StartRanging()
{
    bool rangingActiveExpected = false;
    const bool wasRangingActive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, true);
    if (wasRangingActive) {
        return;
    }

    // TODO: request UWB-CLX to start ranging
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

    // TODO: request UWB-CLX to stop ranging
}


#ifndef UWB_SESSION_HXX
#define UWB_SESSION_HXX

#include <atomic>
#include <cstdint>
#include <unordered_set>

#include <uwb/UwbMacAddress.hxx>

namespace uwb
{
/**
 * @brief Represents a UWB session.
 */
class UwbSession
{
public:
    /**
     * @brief Construct a new UwbSession object.
     * 
     * @param sessionId The associated session id.
     * @param peers The initial set of peers.
     */
    UwbSession(uint32_t sessionId, std::unordered_set<UwbMacAddress> peers);

    /**
     * @brief Get the unique session id.
     * 
     * @return uint32_t 
     */
    uint32_t
    GetId() const noexcept;

    /**
     * @brief 
     */
    void
    StartRanging();

    /**
     * @brief 
     */
    void 
    StopRanging();

private:
    uint32_t m_sessionId;
    std::atomic<bool> m_rangingActive{ false };
    std::unordered_set<uwb::UwbMacAddress> m_peers{};
};

} // namespace uwb

#endif // UWB_SESSION_HXX

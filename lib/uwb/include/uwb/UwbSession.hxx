
#ifndef UWB_SESSION_HXX
#define UWB_SESSION_HXX

#include <cstdint>

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
     */
    UwbSession(uint32_t sessionId);

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
};

} // namespace uwb

#endif // UWB_SESSION_HXX

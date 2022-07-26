
#ifndef __NEAR_OBJECT_SESSION_HXX__
#define __NEAR_OBJECT_SESSION_HXX__

#include <memory>
#include <mutex>
#include <vector>

namespace nearobject
{
struct NearObjectSessionEventCallbacks;

/**
 * @brief Represents a communication session with a near object.
 */
class NearObjectSession
{
public:
    NearObjectSession(std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks);

    ~NearObjectSession();

    enum class RangingSessionStatus
    {
        Running,
        Error,
        NotSupported,
        MaximumSessionsReached,
    };

    struct StartRangingSessionResult 
    {
        RangingSessionStatus Status;
        // TODO: unique id?
    };

    /**
     * @brief Starts a new ranging session. 
     * 
     * @return StartRangingSessionResult 
     */
    StartRangingSessionResult
    StartRangingSession();

    /**
     * @brief Stops the active ranging session, if applicable. 
     */
    void
    StopRangingSession();

    /**
     * @brief Determines if there is an active ranging session. 
     * 
     * @return true 
     * @return false 
     */
    bool
    IsRangingActive() const noexcept;

private:
    void
    OnSessionClosed();

    /**
     * @brief Create a New Ranging Session object
     * TODO: this probably needs to return a tracking object of some kind.
     * 
     * @return RangingSessionStatus 
     */
    RangingSessionStatus
    CreateNewRangingSession();

    /**
     * @brief Determines if this session supports ranging.
     * 
     * @return true 
     * @return false 
     */
    bool
    IsRangingSupported() const noexcept;

private:
    mutable std::mutex m_rangingStateGate;
    bool m_rangingSessionActive = false;

    std::weak_ptr<NearObjectSessionEventCallbacks> m_eventCallbacks;
};

} // namespace nearobject

#endif // __NEAR_OBJECT_SESSION_HXX__


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
    ~NearObjectSession();

    /**
     * @brief Register a set of callbacks for session events.
     * 
     * @param eventCallbacks 
     */
    void
    RegisterCallbacks(std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks);

    enum class RangingStatus
    {
        Successful,
        Error,
        NotSupported,
        MaximumSessionsReached,
    };

    struct StartRangingSessionResult 
    {
        RangingStatus Status;
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
     * @return RangingStatus 
     */
    RangingStatus
    CreateNewRangingSession();

    /**
     * @brief Determines if this session supports ranging.
     * 
     * @return true 
     * @return false 
     */
    bool
    IsRangingSupported() const noexcept;

    /**
     * @brief Get a copy of the currently registered event callbacks. Note that
     * the returned vector contains resolved std::shared_ptr instances, not
     * std::weak_ptr.
     * 
     * @return std::vector<std::shared_ptr<NearObjectSessionEventCallbacks>> 
     */
    std::vector<std::shared_ptr<NearObjectSessionEventCallbacks>>
    GetEventCallbacks();

private:
    mutable std::mutex m_rangingStateGate;
    bool m_rangingSessionActive = false;

    std::mutex m_eventCallbacksGate;
    std::vector<std::weak_ptr<NearObjectSessionEventCallbacks>> m_eventCallbacks;
};

} // namespace nearobject

#endif // __NEAR_OBJECT_SESSION_HXX__

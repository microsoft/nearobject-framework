
#ifndef __NEAR_OBJECT_SESSION_HXX__
#define __NEAR_OBJECT_SESSION_HXX__

#include <memory>
#include <mutex>
#include <vector>

#include "NearObjectCapabilities.hpp"

namespace nearobject
{
struct NearObjectSessionEventCallbacks;

/**
 * @brief Represents a communication session with a near object.
 */
class NearObjectSession
{
public:
    /**
     * @brief Construct a new NearObjectSession object
     * 
     * @param capabilities The capabilities supported by this session.
     * @param eventCallbacks The callbacks used to signal events from this session.
     */
    NearObjectSession(NearObjectCapabilities capabilities, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks);

    /**
     * @brief Destroy the NearObjectSession object
     */
    ~NearObjectSession();

    /**
     * @brief The status of a ranging session.
     */
    enum class RangingSessionStatus {
        Running,
        Error,
        NotSupported,
        MaximumSessionsReached,
    };

    /**
     * @brief The result from starting a ranging session.
     */
    struct StartRangingSessionResult {
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
    /**
     * @brief Ends this session.
     */
    void
    EndSession();

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

public:
    const NearObjectCapabilities Capabilities;

private:
    mutable std::mutex m_rangingStateGate;
    bool m_rangingSessionActive = false;

    std::weak_ptr<NearObjectSessionEventCallbacks> m_eventCallbacks;
};

} // namespace nearobject

#endif // __NEAR_OBJECT_SESSION_HXX__

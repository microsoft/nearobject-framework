
#ifndef NEAR_OBJECT_SESSION_HXX
#define NEAR_OBJECT_SESSION_HXX

#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

#include "NearObject.hxx"
#include "NearObjectCapabilities.hxx"

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
     * @brief Disable copy and move operations
     */
    NearObjectSession(NearObjectSession&) = delete;
    NearObjectSession(NearObjectSession&&) = delete;
    NearObjectSession&
    operator=(NearObjectSession&) = delete;
    NearObjectSession&
    operator=(NearObjectSession&&) = delete;

    /**
     * @brief Construct a new Near Object Session object
     *
     * @param capabilities The capabilities supported by this session.
     * @param nearObjectPeers The initials peers involved in this session.
     * @param eventCallbacks The callbacks used to signal events from this session.
     */
    NearObjectSession(NearObjectCapabilities capabilities, const std::vector<std::shared_ptr<NearObject>>& nearObjectPeers, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks);

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
    /**
     * @brief Resolves the event callback weak_ptr and executes the provided
     * executor function if the pointer was valid.
     *
     * This handles synchronizing and serializing each event callback invocation
     * to ensure the client receives them in order.
     *
     * @param executor The function to execute if the callback pointer was
     * successfully resolved.
     *
     * @return true If the executor was executed.
     * @return false If the executor was not executed.
     */
    bool
    InvokeEventCallback(const std::function<void(NearObjectSessionEventCallbacks& callbacks)>& executor);

    /**
     * @brief Add a near object peer to this session.
     *
     * @param nearObjectAdded The near object peer to add.
     */
    bool
    AddNearObjectPeer(const std::shared_ptr<NearObject> nearObjectAdded);

    /**
     * @brief Remove a near object peer from this session.
     *
     * @param nearObjectRemoved The near object peer to remove.
     */
    bool
    RemoveNearObjectPeer(const std::shared_ptr<NearObject> nearObjectRemoved);

    /**
     * @brief Ends this session.
     */
    void
    EndSession();

    /**
     * @brief Signal that the properties of the specified NearObject has changed.
     *
     * @param nearObjectChanged The near object that changed.
     */
    void
    NearObjectPropertiesChanged(const std::shared_ptr<NearObject> nearObjectChanged);

    /**
     * @brief Create a New Ranging Session object
     * TODO: this probably needs to return a tracking object of some kind.
     *
     * @return RangingSessionStatus
     */
    RangingSessionStatus
    CreateNewRangingSession();

    struct RangingSession
    {
        RangingSession(std::function<void()> rangingDataUpdated) :
            RangingDataUpdated(rangingDataUpdated)
        {}

        std::function<void()> RangingDataUpdated;
    };

public:
    const NearObjectCapabilities Capabilities;

private:
    mutable std::mutex m_rangingStateGate;
    std::optional<RangingSession> m_rangingSession;

    std::weak_ptr<NearObjectSessionEventCallbacks> m_eventCallbacks;

    std::mutex m_nearObjectPeersGate;
    std::vector<std::shared_ptr<NearObject>> m_nearObjectPeers;
};

} // namespace nearobject

#endif // NEAR_OBJECT_SESSION_HXX

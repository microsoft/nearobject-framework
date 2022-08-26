
#ifndef NEAR_OBJECT_SESSION_HXX
#define NEAR_OBJECT_SESSION_HXX

#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

#include <nearobject/NearObject.hxx>
#include <nearobject/NearObjectCapabilities.hxx>

#include <notstd/task_queue.hxx>

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
    NearObjectSession& operator=(NearObjectSession&) = delete;
    NearObjectSession& operator=(NearObjectSession&&) = delete;

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
     * @brief Get the Capabilities object
     *
     * @return NearObjectCapabilities
     */
    NearObjectCapabilities
    GetCapabilities() const noexcept;

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
    StartRanging();

    /**
     * @brief Stops the active ranging session, if applicable.
     */
    void
    StopRanging();

    /**
     * @brief Determines if there is an active ranging session.
     *
     * @return true
     * @return false
     */
    bool
    IsRangingActive() const noexcept;

protected:
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
     */
    void
    InvokeEventCallback(const std::function<void(NearObjectSessionEventCallbacks& callbacks)>& executor);

    void
    InvokeBlockingEventCallback(const std::function<void(NearObjectSessionEventCallbacks& callbacks)>& executor);

    /**
     * @brief Add a near object peer to this session.
     *
     * @param nearObjectAdded The near object peer to add.
     */
    void
    AddNearObjectPeer(std::shared_ptr<NearObject> nearObjectAdded);

    /**
     * @brief Adds a collection of near object peers to this session.
     * 
     * @param nearObjectsAdded The collection of peers to add. 
     */
    void
    AddNearObjectPeers(std::vector<std::shared_ptr<NearObject>> nearObjectsAdded);
    
    /**
     * @brief Remove a near object peer from this session.
     *
     * @param nearObjectRemoved The near object peer to remove.
     */
    void 
    RemoveNearObjectPeer(std::shared_ptr<NearObject> nearObjectRemoved);

    /**
     * @brief Removes a collection of near object peers from this session.
     * 
     * @param nearObjectsRemoved The collection of peers to remove. 
     */
    void
    RemoveNearObjectPeers(std::vector<std::shared_ptr<NearObject>> nearObjectsRemoved);

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
     * @brief Get the current set of peers in the session.
     * 
     * @return std::vector<std::shared_ptr<NearObject>> 
     */
    std::vector<std::shared_ptr<NearObject>> 
    GetPeers() const noexcept;

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
        explicit RangingSession(std::function<void()> rangingDataUpdated) :
            RangingDataUpdated(std::move(rangingDataUpdated))
        {}

        const std::function<void()> RangingDataUpdated;
    };

private:
    mutable std::mutex m_rangingStateGate;
    std::optional<RangingSession> m_rangingSession;

    const NearObjectCapabilities m_capabilities;
    std::weak_ptr<NearObjectSessionEventCallbacks> m_eventCallbacks;

    mutable std::mutex m_nearObjectPeersGate;
    std::vector<std::shared_ptr<NearObject>> m_nearObjectPeers;

    threading::CLooper m_taskQueue;
};

} // namespace nearobject

#endif // NEAR_OBJECT_SESSION_HXX

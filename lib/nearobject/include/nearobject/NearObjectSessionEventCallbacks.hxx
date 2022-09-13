
#ifndef NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX
#define NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX

#include <memory>
#include <vector>

#include "NearObjectSession.hxx"

namespace nearobject
{
class NearObject;

/**
 * @brief Interface for receiving events from a NearObjectSession. This is the
 * primary method to receive information from a NearObject.
 */
struct NearObjectSessionEventCallbacks
{
    /**
     * @brief Destroy the Near Object Session Event Callbacks object
     */
    virtual ~NearObjectSessionEventCallbacks() = default;

    /**
     * @brief Invoked when the session is ended.
     *
     * @param session The session that ended.
     * TODO: This should probably indicate why it ended.
     */
    virtual void
    OnSessionEnded(NearObjectSession *session) = 0;

    /**
     * @brief Invoked when active ranging starts.
     *
     * @param session
     */
    virtual void
    OnRangingStarted(NearObjectSession *session) = 0;

    /**
     * @brief Invoked when active ranging stops.
     *
     * @param session
     */
    virtual void
    OnRangingStopped(NearObjectSession *session) = 0;

    /**
     * @brief Invoked when the properties of a near object involved in the session changes.
     *
     * @param session
     * @param nearObjectsChanged
     */
    virtual void
    OnNearObjectPropertiesChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> nearObjectsChanged) = 0;

    /**
     * @brief Invoked when membership of one or more NearObject's involved in
     * the session is changed. This can occur when NearObject members are either
     * added or removed to the session.
     *
     * @param session
     * @param nearObjectsAdded
     * @param nearObjectsRemoved
     */
    virtual void
    OnSessionMembershipChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> nearObjectsAdded, const std::vector<std::shared_ptr<NearObject>> nearObjectsRemoved) = 0;
};
} // namespace nearobject

#endif //  NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX

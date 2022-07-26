
#ifndef __NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX__
#define __NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX__

#include <memory>
#include <vector>

#include "NearObjectSession.hxx"

namespace nearobject
{
class NearObject;

struct NearObjectSessionEventCallbacks {
    /**
     * @brief Invoked when the session is ended.
     *
     * @param session The session that ended.
     * TODO: This should probably indicate why it ended.
     */
    virtual void
    OnNearObjectSessionEnded(NearObjectSession *session) = 0;

    /**
     * @brief Invoked when an active ranging session is started.
     *
     * @param session
     */
    virtual void
    OnNearObjectRangingSessionStarted(NearObjectSession *session) = 0;

    /**
     * @brief Invoked when an active ranging session is ended.
     *
     * @param session
     */
    virtual void
    OnNearObjectRangingSessionEnded(NearObjectSession *session) = 0;

    /**
     * @brief Invoked when the properties of a near object involved in the session changes.
     *
     * @param session
     * @param nearObjectsChanged
     */
    virtual void
    OnNearObjectSessionNearObjectPropertiesChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> nearObjectsChanged) = 0;

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
    OnNearObjectSessionMembershipChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> nearObjectsAdded, const std::vector<std::shared_ptr<NearObject>> nearObjectsRemoved) = 0;
};
} // namespace nearobject

#endif //  __NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX__

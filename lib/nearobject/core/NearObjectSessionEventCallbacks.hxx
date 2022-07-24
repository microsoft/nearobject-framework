
#ifndef __NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX__
#define __NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX__

#include "NearObjectSession.hxx"

namespace nearobject
{
struct NearObjectSessionEventCallbacks {
    /**
     * @brief Invoked when the session is ended.
     * 
     * @param session The session that ended.
     * TODO: This should probably indicate why it ended.
     */
    virtual void
    OnNearObjectSessionEnded(NearObjectSession *session) = 0;
};
} // namespace nearobject

#endif //  __NEAR_OBJECT_SESSION_EVENT_CALLBACKS_HXX__

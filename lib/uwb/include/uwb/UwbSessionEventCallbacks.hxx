
#ifndef UWB_SESSION_EVENT_CALLBACKS_HXX
#define UWB_SESSION_EVENT_CALLBACKS_HXX

#include <memory>
#include <vector>

#include <uwb/UwbPeer.hxx>

namespace uwb
{
class UwbSession;

/**
 * @brief The possible reasons for a session ending.
 */
enum class UwbSessionEndReason {
    /**
     * @brief The session owner stopped the session.
     *
     * This is the reason used when the session ends naturally.
     */
    Stopped,

    /**
     * @brief The session was locally canceled.
     */
    Canceled,

    /**
     * @brief The session timed out due to policy.
     */
    Timeout,

    /**
     * @brief The session ended for an unknown or unspecified reason.
     */
    Unspecified,
};

/**
 * @brief Interface for receiving events from a UwbSession. This is the primary
 * method to receive information from near peers.
 * 
 * A pointer to the UwbSession for which the callbacks are bound is provided in
 * each callback. This enables using the same callback object instance with
 * multiple sessions where the pointer value can be used to lookup the
 * associated session for the event. This pointer must not be cached nor used
 * beyond execution of the callback.
 */
struct UwbSessionEventCallbacks
{
    /**
     * @brief Invoked when the session is ended.
     * 
     * @param session The session for which the event occurred.
     * @param reason The reason the session ended.
     */
    virtual void
    OnSessionEnded(UwbSession *session, UwbSessionEndReason reason) = 0;

    /**
     * @brief Invoked when active ranging starts.
     *
     * @param session The session for which the event occurred.
     */
    virtual void
    OnRangingStarted(UwbSession *session) = 0;

    /**
     * @brief Invoked when active ranging stops.
     *
     * @param session The session for which the event occurred.
     */
    virtual void
    OnRangingStopped(UwbSession *session) = 0;

    /**
     * @brief Invoked when the properties of a peer involved in the session
     * changes. This includes the spatial properties of the peer(s).
     * 
     * @param session The session for which the event occurred.
     * @param peersChanged A list of peers whose properties changed.
     */
    virtual void
    OnPeerPropertiesChanged(UwbSession *session, std::vector<UwbPeer> peersChanged) = 0;

    /**
     * @brief Invoked when membership of one or more near peers involved in
     * the session is changed. This can occur when peer members are either
     * added to or removed from the session.
     * 
     * @param session The session for which the event occurred.
     * @param peersAdded A list of peers that were added to the session.
     * @param peersRemoved A list of peers that were removed from the session.
     */
    virtual void
    OnSessionMembershipChanged(UwbSession *session, std::vector<UwbPeer> peersAdded, std::vector<UwbPeer> peersRemoved) = 0;

    /**
     * @brief Destroy the UwbSessionEventCallbacks object, defined to support
     * polymorphic deletion.
     */
    virtual ~UwbSessionEventCallbacks() = default;
};
} // namespace uwb

#endif //  UWB_SESSION_EVENT_CALLBACKS_HXX

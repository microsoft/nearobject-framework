
#ifndef NEAR_OBJECT_CLI_UWB_SESSION_EVENT_CALLBACKS_HXX
#define NEAR_OBJECT_CLI_UWB_SESSION_EVENT_CALLBACKS_HXX

#include <uwb/UwbSessionEventCallbacks.hxx>

namespace nearobject::cli
{
/**
 * @brief Windows-specific parsed CLI data.
 */
struct NearObjectCliUwbSessionEventCallbacks :
    public ::uwb::UwbSessionEventCallbacks
{
    /**
     * @brief Invoked when the session is ended.
     *
     * @param session The session for which the event occurred.
     * @param reason The reason the session ended.
     */
    void
    OnSessionEnded(::uwb::UwbSession *session, ::uwb::UwbSessionEndReason reason);

    /**
     * @brief Invoked when active ranging starts.
     *
     * @param session The session for which the event occurred.
     */
    void
    OnRangingStarted(::uwb::UwbSession *session);

    /**
     * @brief Invoked when active ranging stops.
     *
     * @param session The session for which the event occurred.
     */
    void
    OnRangingStopped(::uwb::UwbSession *session);

    /**
     * @brief Invoked when the properties of a peer involved in the session
     * changes. This includes the spatial properties of the peer(s).
     *
     * @param session The session for which the event occurred.
     * @param peersChanged A list of peers whose properties changed.
     */
    void
    OnPeerPropertiesChanged(::uwb::UwbSession *session, const std::vector<::uwb::UwbPeer> peersChanged);

    /**
     * @brief Invoked when membership of one or more near peers involved in
     * the session is changed. This can occur when peer members are either
     * added to or removed from the session.
     *
     * @param session The session for which the event occurred.
     * @param peersAdded A list of peers that were added to the session.
     * @param peersRemoved A list of peers that were removed from the session.
     */
    void
    OnSessionMembershipChanged(::uwb::UwbSession *session, const std::vector<::uwb::UwbPeer> peersAdded, const std::vector<::uwb::UwbPeer> peersRemoved);
};

} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_UWB_SESSION_EVENT_CALLBACKS_HXX

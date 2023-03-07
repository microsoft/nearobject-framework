
#ifndef UWB_SESSION_EVENT_CALLBACKS_HXX
#define UWB_SESSION_EVENT_CALLBACKS_HXX

#include <memory>
#include <vector>

#include <functional>
#include <uwb/UwbPeer.hxx>
namespace uwb
{
struct UwbSession;

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
 */
struct UwbSessionEventCallbacks
{
    /**
     * @brief Invoked when the session is ended.
     *
     * @param session The session for which the event occurred.
     * @param reason The reason the session ended.
     */
    std::function<void(UwbSessionEndReason reason)> OnSessionEnded;

    /**
     * @brief Invoked when active ranging starts.
     *
     * @param session The session for which the event occurred.
     */
    std::function<void()> OnRangingStarted;

    /**
     * @brief Invoked when active ranging stops.
     *
     * @param session The session for which the event occurred.
     */
    std::function<void()> OnRangingStopped;

    /**
     * @brief Invoked when the properties of a peer involved in the session
     * changes. This includes the spatial properties of the peer(s).
     *
     * @param session The session for which the event occurred.
     * @param peersChanged A list of peers whose properties changed.
     */
    std::function<void(const std::vector<UwbPeer> peersChanged)> OnPeerPropertiesChanged;

    /**
     * @brief Invoked when membership of one or more near peers involved in
     * the session is changed. This can occur when peer members are either
     * added to or removed from the session.
     *
     * @param session The session for which the event occurred.
     * @param peersAdded A list of peers that were added to the session.
     * @param peersRemoved A list of peers that were removed from the session.
     */
    std::function<void(const std::vector<UwbPeer> peersAdded, const std::vector<UwbPeer> peersRemoved)> OnSessionMembershipChanged;
};
} // namespace uwb

#endif //  UWB_SESSION_EVENT_CALLBACKS_HXX

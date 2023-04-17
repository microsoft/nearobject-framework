
#ifndef UWB_REGISTERED_CALLBACKS_HXX
#define UWB_REGISTERED_CALLBACKS_HXX

#include <functional>
#include <variant>
#include <vector>

#include <uwb/UwbPeer.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>

namespace uwb
{
namespace UwbRegisteredSessionEventCallbackTypes
{
/**
 * @brief Invoked when the session is ended.
 *
 * @param reason The reason the session ended.
 */
using OnSessionEnded = std::function<void(::uwb::UwbSessionEndReason reason)>;

/**
 * @brief Invoked when active ranging starts.
 *
 */
using OnRangingStarted = std::function<void()>;

/**
 * @brief Invoked when active ranging stops.
 *
 */
using OnRangingStopped = std::function<void()>;

/**
 * @brief Invoked when the properties of a peer involved in the session
 * changes. This includes the spatial properties of the peer(s).
 *
 * @param peersChanged A list of peers whose properties changed.
 */
using OnPeerPropertiesChanged = std::function<void(const std::vector<UwbPeer> peersChanged)>;

/**
 * @brief Invoked when membership of one or more near peers involved in
 * the session is changed. This can occur when peer members are either
 * added to or removed from the session.
 *
 * @param peersAdded A list of peers that were added to the session.
 * @param peersRemoved A list of peers that were removed from the session.
 */
using OnSessionMembershipChanged = std::function<void(const std::vector<UwbPeer> peersAdded, const std::vector<UwbPeer> peersRemoved)>;
}; // namespace UwbRegisteredSessionEventCallbackTypes

namespace UwbRegisteredDeviceEventCallbackTypes
{
/**
 * @brief Invoked when a generic error occurs.
 *
 * @param status The generic error that occurred.
 */
using OnStatusChanged = std::function<void(::uwb::protocol::fira::UwbStatus)>;

/**
 * @brief Invoked when the device status changes.
 *
 * @param statusDevice
 */
using OnDeviceStatusChanged = std::function<void(::uwb::protocol::fira::UwbStatusDevice)>;

/**
 * @brief Invoked when the status of a session changes.
 *
 * @param statusSession The new status of the session.
 */
using OnSessionStatusChanged = std::function<void(::uwb::protocol::fira::UwbSessionStatus)>;
}; // namespace UwbRegisteredDeviceEventCallbackTypes

/**
 * @brief Interface for receiving events from a UwbSession. This is the primary
 * method to receive information from near peers.
 * IMPORTANT The client should save somewhere the shared_ptr to the callbacks that they register.
 */
struct UwbRegisteredSessionEventCallbacks
{
    /**
     * @brief Invoked when the session is ended.
     *
     * @param reason The reason the session ended.
     */
    std::weak_ptr<UwbRegisteredSessionEventCallbackTypes::OnSessionEnded> OnSessionEnded;

    /**
     * @brief Invoked when active ranging starts.
     *
     */
    std::weak_ptr<UwbRegisteredSessionEventCallbackTypes::OnRangingStarted> OnRangingStarted;

    /**
     * @brief Invoked when active ranging stops.
     *
     */
    std::weak_ptr<UwbRegisteredSessionEventCallbackTypes::OnRangingStopped> OnRangingStopped;

    /**
     * @brief Invoked when the properties of a peer involved in the session
     * changes. This includes the spatial properties of the peer(s).
     *
     * @param peersChanged A list of peers whose properties changed.
     */
    std::weak_ptr<UwbRegisteredSessionEventCallbackTypes::OnPeerPropertiesChanged> OnPeerPropertiesChanged;

    /**
     * @brief Invoked when membership of one or more near peers involved in
     * the session is changed. This can occur when peer members are either
     * added to or removed from the session.
     *
     * @param peersAdded A list of peers that were added to the session.
     * @param peersRemoved A list of peers that were removed from the session.
     */
    std::weak_ptr<UwbRegisteredSessionEventCallbackTypes::OnSessionMembershipChanged> OnSessionMembershipChanged;
};

/**
 * @brief Interface for receiving events from a UwbDevice. This is the primary
 * method to receive event information from the device driver.
 */
struct UwbRegisteredDeviceEventCallbacks
{
    /**
     * @brief Invoked when a generic error occurs.
     *
     * @param status The generic error that occurred.
     */
    std::weak_ptr<UwbRegisteredDeviceEventCallbackTypes::OnStatusChanged> OnStatusChanged;

    /**
     * @brief Invoked when the device status changes.
     *
     * @param statusDevice
     */
    std::weak_ptr<UwbRegisteredDeviceEventCallbackTypes::OnDeviceStatusChanged> OnDeviceStatusChanged;

    /**
     * @brief Invoked when the status of a session changes.
     *
     * @param statusSession The new status of the session.
     */
    std::weak_ptr<UwbRegisteredDeviceEventCallbackTypes::OnSessionStatusChanged> OnSessionStatusChanged;
};

/**
 * @brief Opaque class forward declaration to help with the deregistration
 *
 */
class RegisteredCallbackToken;

/**
 * @brief structure containing the tokens corresponding to the callbacks you register
 *
 */
struct UwbRegisteredSessionEventCallbackTokens
{
    std::weak_ptr<RegisteredCallbackToken> OnSessionEndedToken;
    std::weak_ptr<RegisteredCallbackToken> OnRangingStartedToken;
    std::weak_ptr<RegisteredCallbackToken> OnRangingStoppedToken;
    std::weak_ptr<RegisteredCallbackToken> OnPeerPropertiesChangedToken;
    std::weak_ptr<RegisteredCallbackToken> OnSessionMembershipChangedToken;
};

/**
 * @brief structure containing the tokens corresponding to the callbacks you register
 *
 */
struct UwbRegisteredDeviceEventCallbackTokens
{
    std::weak_ptr<RegisteredCallbackToken> OnStatusChangedToken;
    std::weak_ptr<RegisteredCallbackToken> OnDeviceStatusChangedToken;
    std::weak_ptr<RegisteredCallbackToken> OnSessionStatusChangedToken;
};

} // namespace uwb

#endif //  UWB_REGISTERED_CALLBACKS_HXX

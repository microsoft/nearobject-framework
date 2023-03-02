
#ifndef UWB_DEVICE_EVENT_CALLBACKS_HXX
#define UWB_DEVICE_EVENT_CALLBACKS_HXX

#include <functional>

#include <uwb/protocols/fira/FiraDevice.hxx>

namespace uwb
{
/**
 * @brief Interface for receiving events from a UwbDevice. This is the primary
 * method to receive event information from the device driver.
 */
struct UwbDeviceEventCallbacks
{
    /**
     * @brief Invoked when a generic error occurs.
     *
     * @param status The generic error that occurred.
     */
    std::function<void(::uwb::protocol::fira::UwbStatus)> OnStatusChanged;

    /**
     * @brief Invoked when the device status changes.
     *
     * @param statusDevice
     */
    std::function<void(::uwb::protocol::fira::UwbStatusDevice)> OnDeviceStatusChanged;

    /**
     * @brief Invoked when the status of a session changes.
     *
     * @param statusSession The new status of the session.
     */
    std::function<void(::uwb::protocol::fira::UwbSessionStatus)> OnSessionStatusChanged;

    /**
     * @brief Invoked when the multicast list for a session has a status update.
     *
     * @param statusMulticastList The status of the session's multicast list.
     */
    std::function<void(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus)> OnSessionMulticastListStatus;

    /**
     * @brief Invoked when a session has a ranging data update.
     *
     * @param rangingData The new ranging data.
     */
    std::function<void(::uwb::protocol::fira::UwbRangingData)> OnSessionRangingData;
};
} // namespace uwb

#endif //  UWB_DEVICE_EVENT_CALLBACKS_HXX

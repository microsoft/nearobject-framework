
#ifndef UWB_DEVICE_EVENT_CALLBACKS_HXX
#define UWB_DEVICE_EVENT_CALLBACKS_HXX

#include <uwb/protocols/fira/FiraDevice.hxx>

namespace uwb
{
/**
 * @brief Interface for receiving events from a UwbDevice. This is the primary
 * method to receive information from the device driver.
 */
struct UwbDeviceEventCallbacks
{
    /**
     * @brief Destroy the UwbDeviceEventCallbacks object, defined to support
     * polymorphic deletion.
     */
    virtual ~UwbDeviceEventCallbacks() = default;

    /**
     * @brief Invoked when a generic error occurs. 
     * 
     * @param status The generic error that occurred.
     */
    virtual void
    OnStatusChanged(::uwb::protocol::fira::UwbStatus status) = 0;

    /**
     * @brief Invoked when the device status changes.
     * 
     * @param statusDevice 
     */
    virtual void 
    OnDeviceStatusChanged(::uwb::protocol::fira::UwbStatusDevice statusDevice) = 0;

    /**
     * @brief Invoked when the status of a session changes.
     * 
     * @param statusSession The new status of the session.
     */
    virtual void
    OnSessionStatusChanged(::uwb::protocol::fira::UwbSessionStatus statusSession) = 0;

    /**
     * @brief Invoked when the multicast list for a session has a status update.
     * 
     * @param statusMulticastList The status of the session's multicast list.
     */
    virtual void
    OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus statusMulticastList) = 0;

    /**
     * @brief Invoked when a session has a ranging data update. 
     * 
     * @param rangingData The new ranging data.
     */
    virtual void
    OnSessionRangingData(::uwb::protocol::fira::UwbRangingData rangingData) = 0;
};
} // namespace uwb

#endif //  UWB_DEVICE_EVENT_CALLBACKS_HXX

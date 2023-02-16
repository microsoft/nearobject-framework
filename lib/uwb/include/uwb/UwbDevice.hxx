
#ifndef UWB_DEVICE_HXX
#define UWB_DEVICE_HXX

#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

namespace uwb
{
class UwbSession;

/**
 * @brief Interface for interacting with a UWB device.
 */
class UwbDevice
{
public:
    /**
     * @brief Creates a new UWB session with no configuration nor peers.
     * 
     * @param callbacks 
     * @return std::shared_ptr<UwbSession> 
     */
    std::shared_ptr<UwbSession>
    CreateSession(std::weak_ptr<UwbSessionEventCallbacks> callbacks);

    /**
     * @brief Get the FiRa capabilities of the device.
     *
     * @return uwb::protocol::fira::UwbCapability
     */
    uwb::protocol::fira::UwbCapability
    GetCapabilities();

    /**
     * @brief Determine if this device is the same as another.
     *
     * @param other
     * @return true
     * @return false
     */
    virtual bool
    IsEqual(const UwbDevice& other) const noexcept = 0;

    /**
     * @brief Destroy the UwbDevice object.
     */
    virtual ~UwbDevice() = default;

private:
    /**
     * @brief Creates a new UWB session with no configuration nor peers.
     * 
     * @param callbacks 
     * @return std::shared_ptr<UwbSession> 
     */
    virtual std::shared_ptr<UwbSession>
    CreateSessionImpl(std::weak_ptr<UwbSessionEventCallbacks> callbacks) = 0;

    /**
     * @brief Get the FiRa capabilities of the device.
     *
     * @return uwb::protocol::fira::UwbCapability
     */
    virtual uwb::protocol::fira::UwbCapability
    GetCapabilitiesImpl() = 0;

protected:
    /**
     * @brief Invoked when a UWB notification has been received.
     *
     * @param uwbNotificationData
     */
    void
    OnUwbNotification(::uwb::protocol::fira::UwbNotificationData uwbNotificationData);

private:
    /**
     * @brief Invoked when a generic error occurs.
     *
     * @param status The generic error that occurred.
     */
    void
    OnStatusChanged(::uwb::protocol::fira::UwbStatus status);

    /**
     * @brief Invoked when the device status changes.
     *
     * @param statusDevice
     */
    void
    OnDeviceStatusChanged(::uwb::protocol::fira::UwbStatusDevice statusDevice);

    /**
     * @brief Invoked when the status of a session changes.
     *
     * @param statusSession The new status of the session.
     */
    void
    OnSessionStatusChanged(::uwb::protocol::fira::UwbSessionStatus statusSession);

    /**
     * @brief Invoked when the multicast list for a session has a status update.
     *
     * @param statusMulticastList The status of the session's multicast list.
     */
    void
    OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus statusMulticastList);

    /**
     * @brief Invoked when a session has a ranging data update.
     *
     * @param rangingData The new ranging data.
     */
    void
    OnSessionRangingData(::uwb::protocol::fira::UwbRangingData rangingData);

private:
    std::shared_mutex m_sessionsGate;
    std::unordered_map<uint32_t, std::weak_ptr<uwb::UwbSession>> m_sessions{};
};

bool
operator==(const UwbDevice&, const UwbDevice&) noexcept;

bool
operator!=(const UwbDevice&, const UwbDevice&) noexcept;

} // namespace uwb

#endif // UWB_DEVICE_HXX

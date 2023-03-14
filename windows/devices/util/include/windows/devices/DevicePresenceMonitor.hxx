
#ifndef WINDOWS_DEVICE_PRESENCE_MONITOR
#define WINDOWS_DEVICE_PRESENCE_MONITOR

#include <functional>
#include <ios>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <notstd/guid.hxx>

// NB: This must come before any other Windows include
#include <windows.h>

#include <cfgmgr32.h>
#include <guiddef.h>

#include <windows/devices/DeviceResource.hxx>

namespace windows::devices
{
/**
 * @brief A device presence (change) event.
 */
enum class DevicePresenceEvent {
    Arrived,
    Departed,
};

/**
 * @brief Device presence monitor. Allows monitoring for presence changes of a
 * specific class device driver.
 */
class DevicePresenceMonitor
{
public:
    /**
     * @brief Creates a new instance for monitoring presence changes of devices
     * with the specified GUID.
     *
     * @param deviceGuid The GUID of the device to monitor for presence changes.
     * @param callback The callback function to invoke when a device presence
     * change event occurs.
     */
    explicit DevicePresenceMonitor(const GUID& deviceGuid, std::function<void(const GUID& deviceGuid, DevicePresenceEvent presenceEvent, std::string deviceName)> callback) noexcept;

    /**
     * @brief Construct a new Device Presence Monitor object
     *
     * @param deviceGuids A list of deveice guids to monitor for presence changes.
     * @param callback The callback function to invoke when a device presence change event occurs.
     */
    explicit DevicePresenceMonitor(std::vector<GUID> deviceGuids, std::function<void(const GUID& deviceGuid, DevicePresenceEvent presenceEvent, std::string deviceName)> callback) noexcept;

    /**
     * @brief Exception thrown when there is an error starting the monitor.
     */
    class StartException :
        public std::exception
    {
    public:
        StartException(const GUID& deviceGuid, CONFIGRET configurationManagerResult);

        /**
         * @brief The device guid associated with the error.
         *
         * @return GUID
         */
        GUID
        Guid() const noexcept;

        /**
         * @brief The Windows Configuration Manager (CM) return value from the
         * failed operation.
         *
         * @return CONFIGRET
         */
        CONFIGRET
        ConfigurationManagerResult() const noexcept;

        /**
         * @brief A description of what caused the failure.
         *
         * @return const char*
         */
        const char*
        what() const noexcept override;

    private:
        GUID m_deviceGuid;
        std::string m_what;
        CONFIGRET m_configurationManagerResult;
    };

    /**
     * @brief Start monitoring for device presence changes.
     */
    void
    Start();

    /**
     * @brief Stop monitoring for device presence changes.
     */
    void
    Stop();

private:
    /**
     * @brief Register for notifications of device driver events.
     */
    void
    RegisterForDeviceClassNotifications();

    /**
     * @brief Unregister from notifications of device driver events.
     */
    void
    UnregisterForDeviceClassNotifications();

    /**
     * @brief Bound callback function for when a class driver event occurs.
     *
     * @param hcmNotificationHandle
     * @param action
     * @param eventData
     * @param eventDataSize
     */
    void
    OnDeviceInterfaceNotification(HCMNOTIFICATION hcmNotificationHandle, CM_NOTIFY_ACTION action, CM_NOTIFY_EVENT_DATA* eventData, DWORD eventDataSize);

    /**
     * @brief Unbound callback function for when a class driver event occurs.
     *
     * @param hcmNotificationHandle
     * @param context
     * @param action
     * @param eventData
     * @param eventDataSize
     * @return DWORD
     */
    static DWORD CALLBACK
    OnDeviceInterfaceNotificationCallback(HCMNOTIFICATION hcmNotificationHandle, void* context, CM_NOTIFY_ACTION action, CM_NOTIFY_EVENT_DATA* eventData, DWORD eventDataSize);

private:
    std::function<void(const GUID& deviceGuid, DevicePresenceEvent presenceEvent, std::string deviceName)> m_callback;
    std::unordered_map<GUID, unique_hcmnotification> m_deviceGuids{};
};
} // namespace windows::devices

#endif // WINDOWS_DEVICE_PRESENCE_MONITOR

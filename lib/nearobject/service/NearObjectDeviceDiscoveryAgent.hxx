
#ifndef __NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_HXX__
#define __NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_HXX__

#include <functional>
#include <memory>
#include <shared_mutex>

namespace nearobject
{
namespace service
{
class NearObjectDevice;

/**
 * @brief The presence of a near object device.
 */
enum class NearObjectDevicePresence {
    Arrived,
    Departed,
};

/**
 * @brief Discovers and monitors NearObjectDevices.
 */
class NearObjectDeviceDiscoveryAgent
{
    /**
     * @brief Construct a new Near Object Device Discovery Agent object
     */
    NearObjectDeviceDiscoveryAgent() = default;

    /**
     * @brief Construct a new Near ObjectDeviceDiscoveryAgent object.
     *
     * @param onDevicePresenceChanged
     */
    explicit NearObjectDeviceDiscoveryAgent(std::function<void(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDevice> deviceChanged)> onDevicePresenceChanged);

    /**
     * @brief Register a callback for device presence change events.
     *
     * The caller must ensure the validity of this callback during the lifetime of this object instance.
     *
     * @param onDevicePresenceChanged The callback to register.
     */
    void
    RegisterDiscoveryEventCallback(std::function<void(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDevice> deviceChanged)> onDevicePresenceChanged);

protected:
    /**
     * @brief Wrapper for safely invoking any device presence changed registered callback.
     *
     * @param presence The presence change that occurred.
     * @param deviceChanged The device the change occurred for.
     */
    void
    DevicePresenceChanged(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDevice> deviceChanged) const noexcept;

private:
    mutable std::shared_mutex m_onDevicePresenceChangedGate;
    std::function<void(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDevice>& deviceChanged)> m_onDevicePresenceChanged;
};
} // namespace service
} // namespace nearobject

#endif __NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_HXX__


#ifndef DEVICE_MANAGER_HXX
#define DEVICE_MANAGER_HXX

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace nearobject
{
namespace service
{
class NearObjectDevice;
class NearObjectDeviceDiscoveryAgent;
enum class NearObjectDevicePresence;

/**
 * @brief Manages devices which implement the NearObjectDevice interface,
 * thereby providing Near Object services.
 *
 * This class is responsible for tracking the presence of such devices, both
 * when new devices ae discovered and when existing devices are removed or
 * become otherwise unavailable.
 */
class NearObjectDeviceManager :
    public std::enable_shared_from_this<NearObjectDeviceManager>
{
public:
    /**
     * @brief Safely create an instance of the device manager.
     *
     * @return std::shared_ptr<NearObjectDeviceManager>
     */
    [[nodiscard]] static std::shared_ptr<NearObjectDeviceManager>
    Create();

    /**
     * @brief Get an instance of this device manager.
     *
     * @return std::shared_ptr<NearObjectDeviceManager>
     */
    std::shared_ptr<NearObjectDeviceManager>
    GetInstance() noexcept;

    /**
     * @brief Adds a new device discovery agent for use.
     *
     * @param discoveryAgent The discovery agent to add.
     */
    void
    AddDiscoveryAgent(std::unique_ptr<NearObjectDeviceDiscoveryAgent> discoveryAgent);

    /**
     * @brief Get the default near object device.
     *
     * In most cases, this is sufficient to carry out basic near object
     * services. If a device with specific capabilities is required, the
     * GetAllDevices() function can be used to obtain the complete collection of
     * known devices and filter them appropriately.
     *
     * @return std::shared_ptr<NearObjectDevice>
     */
    std::shared_ptr<NearObjectDevice>
    GetDefaultDevice() const;

    /**
     * @brief Get a collection of all near object devices.
     *
     * This function allows the caller to further filter the collection based
     * on it's own set of conditions. As such, weak_ptrs are provided since
     * no assumptions are made as to when the caller will actually filter
     * the collection and select a subset for use, thereby promoting the
     * weak pointers to shared pointers.
     *
     * @return std::vector<std::weak_ptr<NearObjectDevice>>
     */
    std::vector<std::weak_ptr<NearObjectDevice>>
    GetAllDevices() const;

protected:
    /**
     * @brief Construct a new NearObjectDeviceManager object.
     */
    NearObjectDeviceManager() = default;

private:
    /**
     * @brief Callback function for all device agent presence change events.
     *
     * @param discoveryAgent
     * @param presence
     * @param deviceChanged
     */
    void
    OnDevicePresenceChanged(NearObjectDeviceDiscoveryAgent *discoveryAgent, NearObjectDevicePresence presence, std::shared_ptr<NearObjectDevice> deviceChanged);

    /**
     * @brief Adds a new device for use by the framework to implement Near
     * Object services.
     *
     * @param nearObjectDevice The device to add.
     */
    void
    AddDevice(std::shared_ptr<NearObjectDevice> nearObjectDevice);

    /**
     * @brief Removes an existing device from use.
     *
     * @param nearObjectDevice The device to remove.
     */
    void
    RemoveDevice(std::shared_ptr<NearObjectDevice> nearObjectDevice);

private:
    mutable std::mutex m_nearObjectDeviceGate;
    std::vector<std::shared_ptr<NearObjectDevice>> m_nearObjectDevices{};

    mutable std::shared_mutex m_discoveryAgentsGate;
    std::vector<std::unique_ptr<NearObjectDeviceDiscoveryAgent>> m_discoveryAgents{};
};

} // namespace service
} // namespace nearobject

#endif // DEVICE_MANAGER_HXX

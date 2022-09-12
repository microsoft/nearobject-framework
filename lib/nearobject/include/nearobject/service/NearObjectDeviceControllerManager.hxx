
#ifndef NEAR_OBJECT_DEVICE_CONTROLLER_MANAGER_HXX
#define NEAR_OBJECT_DEVICE_CONTROLLER_MANAGER_HXX

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace nearobject
{
namespace service
{
class NearObjectDeviceController;
class NearObjectDeviceControllerDiscoveryAgent;
enum class NearObjectDevicePresence;

/**
 * @brief Manages devices which implement the NearObjectDeviceController interface,
 * thereby providing Near Object services.
 *
 * This class is responsible for tracking the presence of such devices, both
 * when new devices ae discovered and when existing devices are removed or
 * become otherwise unavailable.
 */
class NearObjectDeviceControllerManager :
    public std::enable_shared_from_this<NearObjectDeviceControllerManager>
{
public:
    /**
     * @brief Safely create an instance of the device manager.
     *
     * @return std::shared_ptr<NearObjectDeviceControllerManager>
     */
    [[nodiscard]] static std::shared_ptr<NearObjectDeviceControllerManager>
    Create();

    /**
     * @brief Get an instance of this device manager.
     *
     * @return std::shared_ptr<NearObjectDeviceControllerManager>
     */
    std::shared_ptr<NearObjectDeviceControllerManager>
    GetInstance() noexcept;

    /**
     * @brief Adds a new device discovery agent for use.
     *
     * @param discoveryAgent The discovery agent to add.
     */
    void
    AddDiscoveryAgent(std::unique_ptr<NearObjectDeviceControllerDiscoveryAgent> discoveryAgent);

    /**
     * @brief Get the default near object device.
     *
     * In most cases, this is sufficient to carry out basic near object
     * services. If a device with specific capabilities is required, the
     * GetAllDevices() function can be used to obtain the complete collection of
     * known devices and filter them appropriately.
     *
     * @return std::shared_ptr<NearObjectDeviceController>
     */
    std::shared_ptr<NearObjectDeviceController>
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
     * @return std::vector<std::weak_ptr<NearObjectDeviceController>>
     */
    std::vector<std::weak_ptr<NearObjectDeviceController>>
    GetAllDevices() const;

protected:
    /**
     * @brief Construct a new NearObjectDeviceControllerManager object.
     */
    NearObjectDeviceControllerManager() = default;

private:
    /**
     * @brief Callback function for all device agent presence change events.
     *
     * @param discoveryAgent
     * @param presence
     * @param deviceChanged
     */
    void
    OnDevicePresenceChanged(NearObjectDeviceControllerDiscoveryAgent *discoveryAgent, NearObjectDevicePresence presence, std::shared_ptr<NearObjectDeviceController> deviceChanged);

    /**
     * @brief Adds a new device for use by the framework to implement Near
     * Object services.
     *
     * @param nearObjectDevice The device to add.
     */
    void
    AddDevice(std::shared_ptr<NearObjectDeviceController> nearObjectDevice);

    /**
     * @brief Removes an existing device from use.
     *
     * @param nearObjectDevice The device to remove.
     */
    void
    RemoveDevice(std::shared_ptr<NearObjectDeviceController> nearObjectDevice);

private:
    mutable std::mutex m_nearObjectDeviceGate;
    std::vector<std::shared_ptr<NearObjectDeviceController>> m_nearObjectDevices{};

    mutable std::shared_mutex m_discoveryAgentsGate;
    std::vector<std::unique_ptr<NearObjectDeviceControllerDiscoveryAgent>> m_discoveryAgents{};
};

} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_DEVICE_CONTROLLER_MANAGER_HXX

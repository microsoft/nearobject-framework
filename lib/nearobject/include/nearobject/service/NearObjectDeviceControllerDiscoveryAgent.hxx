
#ifndef NEAR_OBJECT_DEVICE_CONTROLLER_DISCOVERY_AGENT_HXX
#define NEAR_OBJECT_DEVICE_CONTROLLER_DISCOVERY_AGENT_HXX

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <shared_mutex>

namespace nearobject
{
namespace service
{
class NearObjectDeviceController;

/**
 * @brief The presence of a near object device.
 */
enum class NearObjectDevicePresence {
    Arrived,
    Departed,
};

/**
 * @brief Discovers and monitors NearObjectDeviceControllers.
 */
class NearObjectDeviceControllerDiscoveryAgent
{
public:
    /**
     * @brief Construct a new Near Object Device Discovery Agent object
     */
    NearObjectDeviceControllerDiscoveryAgent() = default;

    /**
     * @brief Destroy the Near Object Device Discovery Agent object
     */
    virtual ~NearObjectDeviceControllerDiscoveryAgent() = default;

    /**
     * @brief Construct a new Near ObjectDeviceDiscoveryAgent object.
     *
     * @param onDevicePresenceChanged
     */
    explicit NearObjectDeviceControllerDiscoveryAgent(std::function<void(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDeviceController> deviceChanged)> onDevicePresenceChanged);

    /**
     * @brief Register a callback for device presence change events.
     *
     * The caller must ensure the validity of this callback during the lifetime of this object instance.
     *
     * @param onDevicePresenceChanged The callback to register.
     */
    void
    RegisterDiscoveryEventCallback(std::function<void(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDeviceController> deviceChanged)> onDevicePresenceChanged);

    /**
     * @brief indicates the started/running state.
     *
     * @return true
     * @return false
     */
    bool
    IsStarted() const noexcept;

    /**
     * @brief Start actively discovering devices.
     */
    void
    Start();

    /**
     * @brief Stop actively discovering devices.
     */
    void
    Stop();

    /**
     * @brief Probe for all existing devices.
     * 
     * @return std::future<std::vector<std::shared_ptr<NearObjectDeviceController>>> 
     */
    std::future<std::vector<std::shared_ptr<NearObjectDeviceController>>>
    ProbeAsync();

protected:
    /**
     * @brief Wrapper for safely invoking any device presence changed registered callback.
     *
     * @param presence The presence change that occurred.
     * @param deviceChanged The device the change occurred for.
     */
    void
    DevicePresenceChanged(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDeviceController> deviceChanged) const noexcept;

protected:
    /**
     * @brief Derived class implementation of discovery start.
     */
    virtual void
    StartImpl();

    /**
     * @brief Derived class implementation of discovery stop.
     */
    virtual void
    StopImpl();

    /**
     * @brief Derived class implementation of asynchronous discovery probe.
     *
     * @return std::future<std::vector<std::shared_ptr<NearObjectDeviceController>>>
     */
    virtual std::future<std::vector<std::shared_ptr<NearObjectDeviceController>>>
    ProbeAsyncImpl();

private:
    std::atomic<bool> m_started{ false };

    mutable std::shared_mutex m_onDevicePresenceChangedGate;
    std::function<void(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDeviceController> deviceChanged)> m_onDevicePresenceChanged;
};
} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_DEVICE_CONTROLLER_DISCOVERY_AGENT_HXX

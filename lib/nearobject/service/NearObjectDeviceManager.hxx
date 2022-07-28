
#ifndef __DEVICE_MANAGER_HXX__
#define __DEVICE_MANAGER_HXX__

#include <memory>
#include <mutex>
#include <vector>

namespace nearobject
{
namespace service
{
class NearObjectDevice;

/**
 * @brief Manages devices which implement the NearObjectDevice interface,
 * thereby providing Near Object services.
 * 
 * This class is responsible for tracking the presence of such devices, both
 * when new devices ae discovered and when existing devices are removed or
 * become otherwise unavailable.
 */
class NearObjectDeviceManager
{
public:
    /**
     * @brief Adds a new device for use by the framework to implement Near
     * Object services.
     * 
     * @param nearObjectDevice The device to add.
     */
    void
    AddDevice(std::shared_ptr<NearObjectDevice> nearObjectDevice);

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

private:
    mutable std::mutex m_nearObjectDeviceGate;
    std::vector<std::shared_ptr<NearObjectDevice>> m_nearObjectDevices{};
};

} // namespace service
} // namespace nearobject

#endif // __DEVICE_MANAGER_HXX__


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

class NearObjectDeviceManager
{
    /**
     * @brief Adds a new device for monitoring.
     */
    void
    AddDevice(std::shared_ptr<NearObjectDevice> nearObjectDevice);

private:
    std::mutex m_nearObjectDeviceGate;
    std::vector<std::shared_ptr<NearObjectDevice>> m_nearObjectDevices{};
};

} // namespace service
} // namespace nearobject

#endif // __DEVICE_MANAGER_HXX__

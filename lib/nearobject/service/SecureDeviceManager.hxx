
#ifndef __SECURE_DEVICE_MANAGER_HXX__
#define __SECURE_DEVICE_MANAGER_HXX__

#include <memory>
#include <mutex>
#include <vector>

namespace nearobject
{
namespace service
{
class SecureDevice;

class SecureDeviceManager
{
private:
    /**
     * @brief Adds a new secure device for monitoring. 
     */
    void
    AddSecureDevice(std::shared_ptr<SecureDevice> secureDevice);

private:
    std::mutex m_secureDevicesGate;
    std::vector<std::shared_ptr<SecureDevice>> m_secureDevices{};
};

} // namespace service
} // namespace nearobject

#endif // __SECURE_DEVICE_MANAGER_HXX__

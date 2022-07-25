
#include "SecureDeviceManager.hxx"
#include "SecureDevice.hxx"

using namespace nearobject::service;

void
SecureDeviceManager::AddSecureDevice(std::shared_ptr<SecureDevice> secureDevice)
{
    auto secureDevicesLock = std::scoped_lock{ m_secureDevicesGate };
    m_secureDevices.push_back(std::move(secureDevice));
}

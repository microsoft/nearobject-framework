
#include <algorithm>

#include "NearObjectDeviceManager.hxx"
#include "NearObjectDevice.hxx"

using namespace nearobject::service;

void
NearObjectDeviceManager::AddDevice(std::shared_ptr<NearObjectDevice> nearObjectDevice)
{
    auto nearObjectDevicesLock = std::scoped_lock{ m_nearObjectDeviceGate };
    auto nearObjectDeviceExists = std::any_of(std::cbegin(m_nearObjectDevices), std::cend(m_nearObjectDevices), [&](const auto& nearObjectDeviceExisting) {
        return nearObjectDevice->IsSame(*nearObjectDeviceExisting);
    });

    if (nearObjectDeviceExists) {
        return;
    }

    m_nearObjectDevices.push_back(std::move(nearObjectDevice));
}

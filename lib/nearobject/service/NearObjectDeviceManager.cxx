
#include <algorithm>
#include <iterator>

#include "NearObjectDeviceManager.hxx"
#include "NearObjectDevice.hxx"

using namespace nearobject::service;

void
NearObjectDeviceManager::AddDevice(std::shared_ptr<NearObjectDevice> nearObjectDevice)
{
    const auto nearObjectDevicesLock = std::scoped_lock{ m_nearObjectDeviceGate };
    const auto nearObjectDeviceExists = std::any_of(std::cbegin(m_nearObjectDevices), std::cend(m_nearObjectDevices), [&](const auto& nearObjectDeviceExisting) {
        return /* TODO */ false;
    });

    if (nearObjectDeviceExists) {
        return;
    }

    m_nearObjectDevices.push_back(std::move(nearObjectDevice));
}

std::shared_ptr<NearObjectDevice>
NearObjectDeviceManager::GetDefaultDevice() const
{
    const auto nearObjectDevicesLock = std::scoped_lock{ m_nearObjectDeviceGate };

    return m_nearObjectDevices.empty() 
        ? nullptr
        : m_nearObjectDevices.front(); 
}

std::vector<std::weak_ptr<NearObjectDevice>>
NearObjectDeviceManager::GetAllDevices() const
{
    const auto nearObjectDevicesLock = std::scoped_lock{ m_nearObjectDeviceGate };

    std::vector<std::weak_ptr<NearObjectDevice>> nearObjectDevices{};
    std::transform(std::cbegin(m_nearObjectDevices), std::cend(m_nearObjectDevices), std::back_inserter(nearObjectDevices), [](const auto& nearObjectDevice)
    {
        // Implicit conversion from std::shared_ptr to std::weak_ptr.
        return nearObjectDevice;
    });

    return nearObjectDevices;
}
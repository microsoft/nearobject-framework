
#include <algorithm>
#include <chrono>
#include <iterator>

#include <notstd/memory.hxx>

#include "NearObjectDevice.hxx"
#include "NearObjectDeviceDiscoveryAgent.hxx"
#include "NearObjectDeviceManager.hxx"

using namespace nearobject::service;

std::shared_ptr<NearObjectDeviceManager>
NearObjectDeviceManager::Create()
{
    return std::make_shared<notstd::enable_make_protected<NearObjectDeviceManager>>();
}

std::shared_ptr<NearObjectDeviceManager>
NearObjectDeviceManager::GetInstance() noexcept
{
    return shared_from_this();
}

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

void
NearObjectDeviceManager::RemoveDevice(std::shared_ptr<NearObjectDevice> nearObjectDevice)
{
    const auto nearObjectDevicesLock = std::scoped_lock{ m_nearObjectDeviceGate };
    const auto nearObjectDeviceToRemove = std::find_if(std::cbegin(m_nearObjectDevices), std::cend(m_nearObjectDevices), [&](const auto& nearObjectDeviceExisting) {
        return (*nearObjectDeviceExisting == *nearObjectDevice);
    });

    if (nearObjectDeviceToRemove == std::cend(m_nearObjectDevices)) {
        return;
    }

    m_nearObjectDevices.erase(nearObjectDeviceToRemove);
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
    std::transform(std::cbegin(m_nearObjectDevices), std::cend(m_nearObjectDevices), std::back_inserter(nearObjectDevices), [](const auto& nearObjectDevice) {
        // Implicit conversion from std::shared_ptr to std::weak_ptr.
        return nearObjectDevice;
    });

    return nearObjectDevices;
}

void
NearObjectDeviceManager::AddDiscoveryAgent(std::unique_ptr<NearObjectDeviceDiscoveryAgent> discoveryAgent)
{
    using namespace std::chrono_literals;

    // Use a weak_ptr below to ensure that the device object manager can
    // be safely destroyed prior to the discovery agent. This allows the
    // callback to be registered indefinitely, safely checking whether this
    // instance is still valid upon each callback invocation.
    discoveryAgent->RegisterDiscoveryEventCallback([discoveryAgentPtr = discoveryAgent.get(), weakThis = std::weak_ptr<NearObjectDeviceManager>(GetInstance())](auto&& presence, auto&& deviceChanged) {
        if (auto strongThis = weakThis.lock()) {
            strongThis->OnDevicePresenceChanged(discoveryAgentPtr, presence, deviceChanged);
        }
    });

    // If this agent has already started, kick off a probe to ensure any devices
    // already found will be added ot this manager.
    std::future<std::vector<std::weak_ptr<NearObjectDevice>>> existingDevicesProbe;
    const bool isStarted = discoveryAgent->IsStarted();
    if (isStarted) {
        existingDevicesProbe = discoveryAgent->ProbeAsync();
    } else {
        discoveryAgent->Start();
    }

    // If the agent hasn't yet been started, start it now, then probe for
    // existing devices in case they've already been discovered.
    {
        std::unique_lock<std::shared_mutex> discoveryAgentLock{ m_discoveryAgentsGate };
        m_discoveryAgents.push_back(std::move(discoveryAgent));
    }

    if (existingDevicesProbe.valid()) {
        static constexpr auto probeTimeout = 3s;

        // Wait for the operation to complete.
        const auto waitResult = existingDevicesProbe.wait_for(probeTimeout);

        // If the operation completed, get the results, resolve weak ptrs,
        // and add any valid devices.
        if (waitResult == std::future_status::ready) {
            const auto existingDevices = existingDevicesProbe.get();
            for (const auto& existingDeviceWeak : existingDevices) {
                if (auto existingDevice = existingDeviceWeak.lock()) {
                    AddDevice(std::move(existingDevice));
                }
            }
        } else {
            // TODO: log error
        }
    }
}

void
NearObjectDeviceManager::OnDevicePresenceChanged(NearObjectDeviceDiscoveryAgent* discoveryAgent, NearObjectDevicePresence presence, std::shared_ptr<NearObjectDevice> deviceChanged)
{
    switch (presence) {
        case NearObjectDevicePresence::Arrived:
            AddDevice(std::move(deviceChanged));
            break;
        case NearObjectDevicePresence::Departed:
            RemoveDevice(deviceChanged);
            break;
        default:
            break;
    }
}

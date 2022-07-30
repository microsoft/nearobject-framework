
#include "NearObjectDeviceDiscoveryAgent.hxx"

using namespace nearobject::service;

NearObjectDeviceDiscoveryAgent::NearObjectDeviceDiscoveryAgent(std::function<void(NearObjectDevicePresence presence, const std::shared_ptr<NearObjectDevice> deviceChanged)> onDevicePresenceChanged) :
    m_onDevicePresenceChanged(std::move(onDevicePresenceChanged))
{}

void
NearObjectDeviceDiscoveryAgent::RegisterDiscoveryEventCallback(std::function<void(NearObjectDevicePresence presence, const std::shared_ptr<NearObjectDevice> deviceChanged)> onDevicePresenceChanged)
{
    std::unique_lock<std::shared_mutex> onDevicePresenceChangedLock{ m_onDevicePresenceChangedGate };
    m_onDevicePresenceChanged = std::move(onDevicePresenceChanged);
}

void
NearObjectDeviceDiscoveryAgent::DevicePresenceChanged(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDevice> deviceChanged) const noexcept
{
    std::shared_lock<std::shared_mutex> onDevicePresenceChangedLock{ m_onDevicePresenceChangedGate };
    if (m_onDevicePresenceChanged)
    {
        m_onDevicePresenceChanged(presence, std::move(deviceChanged));
    }
}

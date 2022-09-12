
#include <nearobject/service/NearObjectDeviceControllerDiscoveryAgent.hxx>

using namespace nearobject::service;

NearObjectDeviceControllerDiscoveryAgent::NearObjectDeviceControllerDiscoveryAgent(std::function<void(NearObjectDevicePresence presence, const std::shared_ptr<NearObjectDeviceController> deviceChanged)> onDevicePresenceChanged) :
    m_onDevicePresenceChanged(std::move(onDevicePresenceChanged))
{}

void
NearObjectDeviceControllerDiscoveryAgent::RegisterDiscoveryEventCallback(std::function<void(NearObjectDevicePresence presence, const std::shared_ptr<NearObjectDeviceController> deviceChanged)> onDevicePresenceChanged)
{
    std::unique_lock<std::shared_mutex> onDevicePresenceChangedLock{ m_onDevicePresenceChangedGate };
    m_onDevicePresenceChanged = std::move(onDevicePresenceChanged);
}

void
NearObjectDeviceControllerDiscoveryAgent::DevicePresenceChanged(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDeviceController> deviceChanged) const noexcept
{
    std::shared_lock<std::shared_mutex> onDevicePresenceChangedLock{ m_onDevicePresenceChangedGate };
    if (m_onDevicePresenceChanged) {
        m_onDevicePresenceChanged(presence, std::move(deviceChanged));
    }
}

bool
NearObjectDeviceControllerDiscoveryAgent::IsStarted() const noexcept
{
    return m_started;
}

void
NearObjectDeviceControllerDiscoveryAgent::Start()
{
    bool expected = false;
    if (m_started.compare_exchange_weak(expected, true)) {
        StartImpl();
    }
}

void
NearObjectDeviceControllerDiscoveryAgent::Stop()
{
    bool expected = true;
    if (m_started.compare_exchange_weak(expected, false)) {
        StopImpl();
    }
}

std::future<std::vector<std::shared_ptr<NearObjectDeviceController>>>
NearObjectDeviceControllerDiscoveryAgent::ProbeAsync()
{
    return ProbeAsyncImpl();
}

void
NearObjectDeviceControllerDiscoveryAgent::StartImpl()
{}

void
NearObjectDeviceControllerDiscoveryAgent::StopImpl()
{}

std::future<std::vector<std::shared_ptr<NearObjectDeviceController>>>
NearObjectDeviceControllerDiscoveryAgent::ProbeAsyncImpl()
{
    std::promise<std::vector<std::shared_ptr<NearObjectDeviceController>>> probePromise{};
    probePromise.set_value({});
    return probePromise.get_future();
}


#include "Service.hxx"

using namespace nearobject::service;

void
Service::AddDeviceManager(std::unique_ptr<NearObjectDeviceManager> nearObjectDeviceManager)
{
    m_nearObjectDeviceManagers.push_back(std::move(nearObjectDeviceManager));
}

void
Service::Start()
{
    m_threadMain = std::thread([&]() {
        Run();
    });
}

void
Service::Stop()
{
}

void
Service::HandleEvent()
{
}

void
Service::Run()
{
    m_running = true;

    while (m_running) {
        auto runEventLock = std::unique_lock(m_runEventGate);
        m_runEvent.wait(runEventLock);
        if (!m_running) {
            break;
        }

        HandleEvent();
    }
}

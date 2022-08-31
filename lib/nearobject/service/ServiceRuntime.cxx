
#include <nearobject/service/NearObjectService.hxx>
#include <nearobject/service/ServiceRuntime.hxx>

using namespace nearobject::service;

ServiceRuntime::~ServiceRuntime()
{
    Stop();

    if (m_threadMain.joinable()) {
        m_threadMain.join(); 
    }
}

ServiceRuntime&
ServiceRuntime::SetServiceInstance(std::shared_ptr<NearObjectService> service)
{
    m_service = std::move(service);
    return *this;
}

void
ServiceRuntime::Start()
{
    {
        bool running = false;
        auto runEventLock = std::unique_lock(m_runEventGate);
        if (!m_running.compare_exchange_weak(running, true)) {
            return;
        }
    }

    m_threadMain = std::thread([&]() {
        Run();
    });
}

void
ServiceRuntime::Stop()
{
    {
        bool running = true;
        if (!m_running.compare_exchange_weak(running, false)) {
            return;
        }
    }

    m_runEvent.notify_one();
}

void
ServiceRuntime::HandleEvent()
{
}

void
ServiceRuntime::Run()
{
    while (m_running) {
        auto runEventLock = std::unique_lock(m_runEventGate);
        m_runEvent.wait(runEventLock);
        if (!m_running) {
            break;
        }

        HandleEvent();
    }
}

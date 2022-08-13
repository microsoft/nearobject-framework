
#include <nearobject/service/ServiceRuntime.hxx>

using namespace nearobject::service;

void
ServiceRuntime::Start()
{
    m_threadMain = std::thread([&]() {
        Run();
    });
}

void
ServiceRuntime::Stop()
{
}

void
ServiceRuntime::HandleEvent()
{
}

void
ServiceRuntime::Run()
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


#include "UwbService.hxx"

void
UwbService::Start()
{
    m_threadMain = std::jthread([&]() {
        Run();
    });
}

void
UwbService::Stop()
{
}

void
UwbService::HandleEvent()
{
}

void
UwbService::Run()
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

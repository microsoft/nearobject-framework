
#ifndef __SERVICE_HXX__
#define __SERVICE_HXX__

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

#include "NearObjectDeviceManager.hxx"

namespace nearobject
{
namespace service
{
class Service
{
public:
    Service() = default;
    Service(std::unique_ptr<NearObjectDeviceManager> nearObjectDeviceManager);

private:
    std::unique_ptr<NearObjectDeviceManager> m_nearObjectDeviceManager;

public:
    void
    Start();

    void
    Stop();

    void
    HandleEvent();

    void
    Run();

private:
    bool m_running = false;
    std::thread m_threadMain;
    std::mutex m_runEventGate;
    std::condition_variable m_runEvent;
};

} // namespace service
} // namespace nearobject

#endif // __SERVICE_HXX__

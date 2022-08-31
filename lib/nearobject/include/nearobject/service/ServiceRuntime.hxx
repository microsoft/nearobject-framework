
#ifndef SERVICE_RUNTIME_HXX
#define SERVICE_RUNTIME_HXX

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace nearobject
{
namespace service
{
struct NearObjectService;

class ServiceRuntime
{
public:
    ~ServiceRuntime();

    ServiceRuntime&
    SetServiceInstance(std::shared_ptr<NearObjectService> service);

    void
    Start();

    void
    Stop();

    void
    HandleEvent();

    void
    Run();

private:
    std::atomic<bool> m_running = false;
    std::thread m_threadMain;
    std::mutex m_runEventGate;
    std::condition_variable m_runEvent;
    std::shared_ptr<NearObjectService> m_service;
};

} // namespace service
} // namespace nearobject

#endif // SERVICE_RUNTIME_HXX

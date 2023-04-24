
#ifndef SERVICE_RUNTIME_HXX
#define SERVICE_RUNTIME_HXX

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace nearobject::service
{
struct NearObjectService;

class ServiceRuntime
{
public:
    ServiceRuntime() = default;
    ~ServiceRuntime();

    ServiceRuntime&
    operator=(const ServiceRuntime&) = default;
    ServiceRuntime&
    operator=(ServiceRuntime&&) = default;
    ServiceRuntime(ServiceRuntime&) = default;
    ServiceRuntime(ServiceRuntime&&) = default;

    /**
     * @brief Obtain a shared reference to the NearObjectService instance
     * managed by this runtime.
     * 
     * @return std::shared_ptr<NearObjectService> 
     */
    std::shared_ptr<NearObjectService>
    GetServiceInstance();

    /**
     * @brief Set the Service instance object.
     *
     * @param service The near object service to run.
     * @return ServiceRuntime& A reference to this object.
     */
    ServiceRuntime&
    SetServiceInstance(std::shared_ptr<NearObjectService> service);

    /**
     * @brief Start the service main event loop.
     */
    void
    Start();

    /**
     * @brief Stop the service main event loop.
     *
     * This sends a request to stop the event loop and returns immediately. The
     * service will complete handling on any in-flight requests and then exit.
     */
    void
    Stop();

private:
    /**
     * @brief Handle a service event that has occurred.
     */
    void
    HandleEvent();

    /**
     * @brief Main event loop function. This runs, processing events and
     * requests until requested to stop, either explicitly through the Stop()
     * function, or upon destruction of this object.
     */
    void
    Run();

private:
    std::atomic<bool> m_running = false;
    std::jthread m_threadMain;
    std::mutex m_runEventGate;
    std::condition_variable m_runEvent;
    std::shared_ptr<NearObjectService> m_service;
};

} // namespace nearobject::service

#endif // SERVICE_RUNTIME_HXX

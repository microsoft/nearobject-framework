
#ifndef __UWB_SERVICE_HXX__
#define __UWB_SERVICE_HXX__

#include <condition_variable>
#include <mutex>
#include <thread>

namespace windows
{
namespace uwb
{

class UwbService
{
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
    std::jthread m_threadMain;
    std::mutex m_runEventGate;
    std::condition_variable m_runEvent;
};

} // namespace uwb
} // namespace windows

#endif // __UWB_SERVICE_HXX__
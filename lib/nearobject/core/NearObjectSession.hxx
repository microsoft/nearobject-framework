
#ifndef __NEAR_OBJECT_SESSION_HXX__
#define __NEAR_OBJECT_SESSION_HXX__

#include <memory>
#include <mutex>
#include <vector>

#include "NearObjectSessionEventCallbacks.hxx"

namespace nearobject
{
/**
 * @brief Represents a communication session with a near object.
 */
class NearObjectSession
{
public:
    ~NearObjectSession();

    /**
     * @brief Reigster a set of callbacks for session events.
     * 
     * @param eventCallbacks 
     */
    void 
    RegisterCallbacks(std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks);

private:
    void
    OnSessionClosed();

    /**
     * @brief Get a copy of the currently registered event callbacks.
     * 
     * @return std::vector<std::shared_ptr<NearObjectSessionEventCallbacks>> 
     */
    std::vector<std::shared_ptr<NearObjectSessionEventCallbacks>> 
    GetEventCallbacks();

private:
    std::mutex m_eventCallbacksGate;
    std::vector<std::weak_ptr<NearObjectSessionEventCallbacks>> m_eventCallbacks;
};

} // namespace nearobject

#endif // __NEAR_OBJECT_SESSION_HXX__
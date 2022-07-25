
#ifndef __NEAR_OBJECT_SESSION_EVENT_HANDLER_HXX__
#define __NEAR_OBJECT_SESSION_EVENT_HANDLER_HXX__

#include <memory>


namespace nearobject
{
class NearObjectSession;
struct NearObjectSessionEventCallbacks;

namespace service
{
class NearObjectSessionEventHandler final
{
public:
    NearObjectSessionEventHandler(std::shared_ptr<NearObjectSession> session, std::shared_ptr<NearObjectSessionEventCallbacks> callbacks);

private:
    std::shared_ptr<NearObjectSession> m_session{ nullptr };
    std::shared_ptr<NearObjectSessionEventCallbacks> m_callbacks{ nullptr };
};

} // namespace service
} // namespace nearobject

#endif // __NEAR_OBJECT_SESSION_EVENT_HANDLER_HXX__

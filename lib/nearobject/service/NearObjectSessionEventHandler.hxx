
#ifndef __NEAR_OBJECT_SESSION_EVENT_HANDLER_HXX__
#define __NEAR_OBJECT_SESSION_EVENT_HANDLER_HXX__

#include <nearobject/NearObjectSession.hxx>

namespace nearobject
{
struct NearObjectSessionEventCallbacks;

class NearObjectSessionEventHandler final
{
public:
    NearObjectSessionEventHandler(NearObjectSession *session, std::shared_ptr<NearObjectSessionEventCallbacks> callbacks);

private:
    NearObjectSession *m_session{ nullptr };
    std::shared_ptr<NearObjectSessionEventCallbacks> m_callbacks{ nullptr };
};
} // namespace nearobject

#endif // __NEAR_OBJECT_SESSION_EVENT_HANDLER_HXX__

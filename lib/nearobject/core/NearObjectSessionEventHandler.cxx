
#include "NearObjectSessionEventHandler.hxx"
#include "NearObjectSessionEventCallbacks.hxx"

using namespace nearobject;

NearObjectSessionEventHandler::NearObjectSessionEventHandler(NearObjectSession* session, std::shared_ptr<NearObjectSessionEventCallbacks> callbacks) :
    m_session(session),
    m_callbacks(callbacks)
{
    m_session->RegisterCallbacks(callbacks);
}

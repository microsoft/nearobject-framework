
#include <nearobject/NearObjectSessionEventCallbacks.hxx>
#include <nearobject/NearObjectSession.hxx>

#include "NearObjectSessionEventHandler.hxx"

using namespace nearobject::service;

NearObjectSessionEventHandler::NearObjectSessionEventHandler(std::shared_ptr<NearObjectSession> session, std::shared_ptr<NearObjectSessionEventCallbacks> callbacks) :
    m_session(session),
    m_callbacks(callbacks)
{
    m_session->RegisterCallbacks(callbacks);
}

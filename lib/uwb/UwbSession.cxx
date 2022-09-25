
#include <uwb/UwbSession.hxx>

using namespace uwb;

UwbSession::UwbSession(uint32_t sessionId) :
    m_sessionId(sessionId)
{}

uint32_t
UwbSession::GetId() const noexcept
{
    return m_sessionId;
}

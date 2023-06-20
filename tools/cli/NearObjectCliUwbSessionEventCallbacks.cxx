
#include <iomanip>
#include <iostream>
#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>
#include <notstd/tostring.hxx>

#include <magic_enum.hpp>
#include <plog/Log.h>

using namespace nearobject::cli;
using namespace strings::ostream_operators;

NearObjectCliUwbSessionEventCallbacks::NearObjectCliUwbSessionEventCallbacks(std::function<void()> onSessionEndedCallback) :
    m_onSessionEndedCallback(std::move(onSessionEndedCallback))
{}

namespace
{
/**
 * @brief Helper stream manipulator to add a common log prefix.
 */
class LogPrefix
{
public:
    LogPrefix(uint32_t sessionId) :
        m_sessionId(sessionId) {}

    std::ostream&
    operator()(std::ostream& out) const
    {
        return out << "[Session " << std::setw(10) << std::setfill(' ') << std::right << m_sessionId << "]: ";
    }

private:
    uint32_t m_sessionId;
};

/**
 * @brief Stream specialization taking a LogPrefix argument, allowing
 * std::ostream << LogPrefix(value).
 *
 * @param out The stream to manipulate.
 * @param logPrefix The instance to manipulate the stream.
 * @return std::ostream&
 */
std::ostream&
operator<<(std::ostream& out, LogPrefix logPrefix)
{
    return logPrefix(out);
}
} // namespace

void
NearObjectCliUwbSessionEventCallbacks::OnSessionEnded(::uwb::UwbSession* session, ::uwb::UwbSessionEndReason reason)
{
    PLOG_VERBOSE << LogPrefix(session->GetId()) << "Session Ended (" << magic_enum::enum_name(reason) << ")";

    if (m_onSessionEndedCallback) {
        m_onSessionEndedCallback();
    }
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStarted(::uwb::UwbSession* session)
{
    PLOG_VERBOSE << LogPrefix(session->GetId()) << "Ranging Started";
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStopped(::uwb::UwbSession* session)
{
    PLOG_VERBOSE << LogPrefix(session->GetId()) << "Ranging Stopped";
}

void
NearObjectCliUwbSessionEventCallbacks::OnPeerPropertiesChanged(::uwb::UwbSession* session, std::vector<::uwb::UwbPeer> peersChanged)
{
    PLOG_VERBOSE << LogPrefix(session->GetId()) << "Peer Properties Changed";

    for (const auto& peer : peersChanged) {
        PLOG_VERBOSE << " " << peer.ToString();
    }
}

void
NearObjectCliUwbSessionEventCallbacks::OnSessionMembershipChanged(::uwb::UwbSession* session, std::vector<::uwb::UwbPeer> peersAdded, std::vector<::uwb::UwbPeer> peersRemoved)
{
    PLOG_VERBOSE << LogPrefix(session->GetId()) << "Membership Changed";

    for (const auto& peer : peersAdded) {
        PLOG_VERBOSE << "+" << peer.GetAddress().ToString();
    }
    for (const auto& peer : peersRemoved) {
        PLOG_VERBOSE << "-" << peer.GetAddress().ToString();
    }
}
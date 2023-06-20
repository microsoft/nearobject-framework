
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

void
NearObjectCliUwbSessionEventCallbacks::OnSessionEnded(::uwb::UwbSession* session, ::uwb::UwbSessionEndReason reason)
{
    PLOG_VERBOSE << "Session " << session->GetId() << " ended (" << magic_enum::enum_name(reason) << ")";

    if (m_onSessionEndedCallback) {
        m_onSessionEndedCallback();
    }
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStarted(::uwb::UwbSession* session)
{
    PLOG_VERBOSE << "Session " << session->GetId() << " ranging started";
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStopped(::uwb::UwbSession* session)
{
    PLOG_VERBOSE << "Session " << session->GetId() << " ranging stopped";
}

void
NearObjectCliUwbSessionEventCallbacks::OnPeerPropertiesChanged(::uwb::UwbSession* session, std::vector<::uwb::UwbPeer> peersChanged)
{
    PLOG_VERBOSE << "Session " << session->GetId() << " peer properties changed";

    for (const auto& peer : peersChanged) {
        PLOG_VERBOSE << " " << peer.ToString();
    }
}

void
NearObjectCliUwbSessionEventCallbacks::OnSessionMembershipChanged(::uwb::UwbSession* session, std::vector<::uwb::UwbPeer> peersAdded, std::vector<::uwb::UwbPeer> peersRemoved)
{
    PLOG_VERBOSE << "Session " << session->GetId() << " session membership changed";

    for (const auto& peer : peersAdded) {
        PLOG_VERBOSE << "+" << peer.GetAddress().ToString();
    }
    for (const auto& peer : peersRemoved) {
        PLOG_VERBOSE << "-" << peer.GetAddress().ToString();
    }
}

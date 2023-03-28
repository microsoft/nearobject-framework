
#include <iomanip>
#include <iostream>
#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>
#include <notstd/tostring.hxx>

using namespace nearobject::cli;
using namespace strings::ostream_operators;

NearObjectCliUwbSessionEventCallbacks::NearObjectCliUwbSessionEventCallbacks(std::function<void()> onSessionEndedCallback) :
    m_onSessionEndedCallback(std::move(onSessionEndedCallback))
{}

void
NearObjectCliUwbSessionEventCallbacks::OnSessionEnded(::uwb::UwbSession* session, ::uwb::UwbSessionEndReason /* reason */)
{
    std::cout << "Session with id="
              << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Session Ended" << std::endl;
    if (m_onSessionEndedCallback) {
        m_onSessionEndedCallback();
    }
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStarted(::uwb::UwbSession* session)
{
    std::cout << "Session with id="
              << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Ranging Started" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStopped(::uwb::UwbSession* session)
{
    std::cout << "Session with id="
              << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Ranging Stopped" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnPeerPropertiesChanged(::uwb::UwbSession* session, const std::vector<::uwb::UwbPeer> peersChanged)
{
    std::cout << "Session with id="
              << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Peer Properties Changed" << std::endl;

    for (const auto& peer : peersChanged) {
        std::cout << " > " << peer << std::endl;
    }
}

void
NearObjectCliUwbSessionEventCallbacks::OnSessionMembershipChanged(::uwb::UwbSession* session, const std::vector<::uwb::UwbPeer> peersAdded, const std::vector<::uwb::UwbPeer> peersRemoved)
{
    std::cout << "Session with id="
              << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Membership Changed" << std::endl;

    for (const auto& peer : peersAdded) {
        std::cout << "+" << peer.GetAddress() << std::endl;
    }
    for (const auto& peer : peersRemoved) {
        std::cout << "-" << peer.GetAddress() << std::endl;
    }
}
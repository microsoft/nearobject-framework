
#include <iostream>

#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>

using namespace nearobject::cli;

void
NearObjectCliUwbSessionEventCallbacks::OnSessionEnded(::uwb::UwbSession* session, ::uwb::UwbSessionEndReason reason)
{
    std::cout << "Session " << session->GetId() << ": Session Ended" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStarted(::uwb::UwbSession* session)
{
    std::cout << "Session " << session->GetId() << ": Ranging Started" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStopped(::uwb::UwbSession* session)
{
    std::cout << "Session " << session->GetId() << ": Ranging Stopped" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnPeerPropertiesChanged(::uwb::UwbSession* session, const std::vector<::uwb::UwbPeer> peersChanged)
{
    std::cout << "Session " << session->GetId() << ": Peer Properties Changed" << std::endl;
    for (const auto& peer : peersChanged) {
        std::cout << peer.ToString() << std::endl;
    }
}

void
NearObjectCliUwbSessionEventCallbacks::OnSessionMembershipChanged(::uwb::UwbSession* session, const std::vector<::uwb::UwbPeer> peersAdded, const std::vector<::uwb::UwbPeer> peersRemoved)
{
    std::cout << "Session " << session->GetId() << ": Membership Changed" << std::endl;
    for (const auto& peer : peersAdded) {
        std::cout << "+" << peer.GetAddress().ToString() << std::endl;
    }
    for (const auto& peer : peersRemoved) {
        std::cout << "-" << peer.GetAddress().ToString() << std::endl;
    }
}
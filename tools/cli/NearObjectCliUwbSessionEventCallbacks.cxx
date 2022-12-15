
#include <iostream>

#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>

using namespace nearobject::cli;

void
NearObjectCliUwbSessionEventCallbacks::OnSessionEnded(::uwb::UwbSession* /* session */, ::uwb::UwbSessionEndReason /* reason */)
{
    std::cout << "Session Ended" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStarted(::uwb::UwbSession* /* session */)
{
    std::cout << "Ranging Started" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnRangingStopped(::uwb::UwbSession* /* session */)
{
    std::cout << "Ranging Stopped" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnPeerPropertiesChanged(::uwb::UwbSession* /* session */, const std::vector<::uwb::UwbPeer> /* peersChanged */)
{
    std::cout << "Peer Properties Changed" << std::endl;
}

void
NearObjectCliUwbSessionEventCallbacks::OnSessionMembershipChanged(::uwb::UwbSession* /* session */, const std::vector<::uwb::UwbPeer> /* peersAdded */, const std::vector<::uwb::UwbPeer> /* peersRemoved */)
{
    std::cout << "Session Membership Changed" << std::endl;
}
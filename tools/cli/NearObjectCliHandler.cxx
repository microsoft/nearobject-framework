#include <iomanip>
#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <notstd/tostring.hxx>
#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSession.hxx>

using namespace nearobject::cli;

std::shared_ptr<uwb::UwbDevice>
NearObjectCliHandler::ResolveUwbDevice(const nearobject::cli::NearObjectCliData& /*cliData */) noexcept
{
    // Default implementation does not know how to resolve a device. OS-specific
    // implementations are expected to sub-class and override this function.
    return nullptr;
}

void
NearObjectCliHandler::HandleStartRanging(std::shared_ptr<uwb::UwbDevice> uwbDevice, uwb::protocol::fira::UwbSessionData& sessionData) noexcept
{
    auto session = uwbDevice->CreateSession();
    auto callbacks = std::make_shared<::uwb::UwbSessionEventCallbacks>(
        [session](::uwb::UwbSessionEndReason) {
            std::cout << "Session with id="
                      << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Session Ended" << std::endl;
        },
        [session]() {
            std::cout << "Session with id="
                      << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Ranging Started" << std::endl;
        },
        [session]() {
            std::cout << "Session with id="
                      << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Ranging Stopped" << std::endl;
        },
        [session](const std::vector<::uwb::UwbPeer> peersChanged) {
            std::cout << "Session with id="
                      << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Peer Properties Changed" << std::endl;

            for (const auto& peer : peersChanged) {
                std::cout << peer.ToString() << std::endl;
            }
        },
        [session](const std::vector<::uwb::UwbPeer> peersAdded, const std::vector<::uwb::UwbPeer> peersRemoved) {
            std::cout << "Session with id="
                      << std::hex << std::setw(8) << std::setfill('0') << std::showbase << std::internal << session->GetId() << ": Membership Changed" << std::endl;

            for (const auto& peer : peersAdded) {
                std::cout << "+" << peer.GetAddress().ToString() << std::endl;
            }
            for (const auto& peer : peersRemoved) {
                std::cout << "-" << peer.GetAddress().ToString() << std::endl;
            }
        });
    session->Configure(sessionData, callbacks);
    session->StartRanging();
}

void
NearObjectCliHandler::HandleStopRanging() noexcept
{
    // TODO
}

void
NearObjectCliHandler::HandleMonitorMode() noexcept
{
    // TODO
}

void
NearObjectCliHandler::HandleDeviceReset(std::shared_ptr<uwb::UwbDevice> uwbDevice) noexcept
{
    uwbDevice->Reset();
}

void
NearObjectCliHandler::HandleGetDeviceInfo(std::shared_ptr<uwb::UwbDevice> uwbDevice) noexcept
{
    auto deviceInfo = uwbDevice->GetDeviceInformation();
    std::cout << deviceInfo.ToString() << std::endl;
}

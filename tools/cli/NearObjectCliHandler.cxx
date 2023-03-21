
#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>
#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSession.hxx>

#include <plog/Log.h>

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
    auto callbacks = std::make_shared<nearobject::cli::NearObjectCliUwbSessionEventCallbacks>();
    auto session = uwbDevice->CreateSession(callbacks);
    if (not sessionData.uwbConfigurationAvailable) {
        PLOG_WARNING << "did not start ranging due to missing configuration";
    } else {
        session->Configure(sessionData.sessionId, sessionData.uwbConfiguration.GetUciConfigParams());
        session->StartRanging();
    }
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

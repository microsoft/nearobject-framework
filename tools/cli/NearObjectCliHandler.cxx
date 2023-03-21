
#include <plog/Log.h>

#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>
#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/protocols/fira/UwbOobConversions.hxx>

#include <plog/Log.h>
#include <uwb/protocols/fira/UwbException.hxx>

using namespace nearobject::cli;
using namespace uwb::protocol::fira;

NearObjectCliHandler::NearObjectCliHandler(std::shared_ptr<NearObjectCliControlFlowContext> controlFlowContext) :
    m_controlFlowContext(std::move(controlFlowContext))
{}

NearObjectCliControlFlowContext&
NearObjectCliHandler::GetControlFlowContext()
{
    return *(m_controlFlowContext.get());
}

std::shared_ptr<uwb::UwbDevice>
NearObjectCliHandler::ResolveUwbDevice(const nearobject::cli::NearObjectCliData& /*cliData */) noexcept
{
    // Default implementation does not know how to resolve a device. OS-specific
    // implementations are expected to sub-class and override this function.
    return nullptr;
}

void
NearObjectCliHandler::HandleStartRanging(std::shared_ptr<uwb::UwbDevice> uwbDevice, uwb::protocol::fira::UwbSessionData& sessionData) noexcept
try {
    auto callbacks = std::make_shared<nearobject::cli::NearObjectCliUwbSessionEventCallbacks>();
    auto session = uwbDevice->CreateSession(callbacks);
    session->Configure(sessionData.sessionId, uwb::protocol::fira::GetUciConfigParams(sessionData.uwbConfiguration, session->GetDeviceType()));
    session->StartRanging();
} catch (...) {
    PLOG_ERROR << "failed to start ranging";
}

void
NearObjectCliHandler::HandleStopRanging() noexcept
try {
    // TODO
} catch (...) {
    PLOG_ERROR << "failed to stop ranging";
}

void
NearObjectCliHandler::HandleMonitorMode() noexcept
try {
    // TODO
} catch (...) {
    PLOG_ERROR << "failed to initiate monitor mode";
}

void
NearObjectCliHandler::HandleDeviceReset(std::shared_ptr<uwb::UwbDevice> uwbDevice) noexcept
try {
    uwbDevice->Reset();
} catch (...) {
    PLOG_ERROR << "failed to reset uwb device";
}

void
NearObjectCliHandler::HandleGetDeviceInfo(std::shared_ptr<uwb::UwbDevice> uwbDevice) noexcept
try {
    auto deviceInfo = uwbDevice->GetDeviceInformation();
    std::cout << deviceInfo.ToString() << std::endl;
} catch (...) {
    PLOG_ERROR << "failed to obtain device information";
}


#include <nearobject/cli/NearObjectCli.hxx>
#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>
#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/protocols/fira/UwbException.hxx>
#include <uwb/protocols/fira/UwbOobConversions.hxx>

#include <plog/Log.h>

using namespace nearobject::cli;
using namespace uwb::protocol::fira;

void
NearObjectCliHandler::SetParent(NearObjectCli* parent)
{
    m_parent = parent;
}

std::shared_ptr<uwb::UwbDevice>
NearObjectCliHandler::ResolveUwbDevice(const nearobject::cli::NearObjectCliData& /*cliData */) noexcept
{
    // Default implementation does not know how to resolve a device. OS-specific
    // implementations are expected to sub-class and override this function.
    return nullptr;
}

void
NearObjectCliHandler::HandleDriverStartRanging(std::shared_ptr<uwb::UwbDevice> uwbDevice, const UwbRangingParameters& rangingParameters) noexcept
try {
    auto controlFlowContext = (m_parent != nullptr) ? m_parent->GetControlFlowContext() : nullptr;
    auto callbacks = std::make_shared<nearobject::cli::NearObjectCliUwbSessionEventCallbacks>([controlFlowContext = std::move(controlFlowContext)]() {
        if (controlFlowContext != nullptr) {
            controlFlowContext->OperationSignalComplete();
        }
    });
    auto session = uwbDevice->CreateSession(rangingParameters.sessionId, callbacks);
    session->Configure(rangingParameters.appConfigParams);
    session->StartRanging();
} catch (...) {
    PLOG_ERROR << "failed to start ranging";
}

void
NearObjectCliHandler::HandleStartRanging(std::shared_ptr<uwb::UwbDevice> uwbDevice, uwb::protocol::fira::UwbSessionData& sessionData) noexcept
try {
    auto controlFlowContext = (m_parent != nullptr) ? m_parent->GetControlFlowContext() : nullptr;
    auto callbacks = std::make_shared<nearobject::cli::NearObjectCliUwbSessionEventCallbacks>([controlFlowContext = std::move(controlFlowContext)]() {
        if (controlFlowContext != nullptr) {
            controlFlowContext->OperationSignalComplete();
        }
    });
    auto session = uwbDevice->CreateSession(sessionData.sessionId, callbacks);
    session->Configure(uwb::protocol::fira::GetUciConfigParams(sessionData.uwbConfiguration, session->GetDeviceType()));
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


#include <exception>

#include <plog/Log.h>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbException.hxx>
#include <windows/devices/uwb/UwbDeviceConnector.hxx>
#include <windows/devices/uwb/simulator/UwbDeviceSimulator.hxx>
#include <windows/devices/uwb/simulator/UwbSessionSimulator.hxx>

using namespace ::uwb::protocol::fira;
using namespace windows::devices::uwb::simulator;

UwbDeviceSimulator::UwbDeviceSimulator(std::string deviceName) :
    UwbDevice(deviceName),
    m_deviceName(std::move(deviceName))
{
    // The simulator driver publishes device interfaces for both itself
    // (GUID_DEVINTERFACE_UWB_SIMULATOR) and UWB (GUID_UWB_DEVICE_INTERFACE). It
    // will respond to requests for both interfaces on each device instance, so
    // we simply push the deviceName to the underlying UWB device.
    // eg. with 'UwbDevice(deviceName)' above.
}

const std::string&
UwbDeviceSimulator::DeviceName() const noexcept
{
    return m_deviceName;
}

bool
UwbDeviceSimulator::Initialize()
{
    m_uwbDeviceConnector = std::make_shared<UwbConnector>(m_deviceName);
    m_uwbDeviceSimulatorConnector = std::make_shared<UwbDeviceSimulatorConnector>(m_deviceName);
    return true;
}

std::shared_ptr<::uwb::UwbSession>
UwbDeviceSimulator::CreateSessionImpl(uint32_t sessionId, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks)
{
    return std::make_shared<UwbSessionSimulator>(sessionId, std::move(callbacks), this, m_uwbDeviceSimulatorConnector);
}

UwbSimulatorCapabilities
UwbDeviceSimulator::GetSimulatorCapabilities()
{
    auto resultFuture = m_uwbDeviceSimulatorConnector->GetCapabilites();
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to obtain simulator capabilities";
        throw UwbException(UwbStatusGeneric::Failed);
    }

    try {
        auto simulatorCapabilities = resultFuture.get();
        return std::move(simulatorCapabilities);
    } catch (const UwbException& e) {
        PLOG_ERROR << "caught exception obtaining simulator capabilities";
        throw e;
    }
}

UwbSimulatorTriggerSessionEventResult
UwbDeviceSimulator::TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs& triggerSessionEventArgs)
{
    auto resultFuture = m_uwbDeviceSimulatorConnector->TriggerSessionEvent(triggerSessionEventArgs);
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to trigger session event";
        throw UwbException(UwbStatusGeneric::Failed);
    }

    try {
        auto uwbSimulatorTriggerSessionEventResult = resultFuture.get();
        return std::move(uwbSimulatorTriggerSessionEventResult);
    } catch (const UwbException& e) {
        PLOG_ERROR << "caught exception triggering session event (" << ToString(e.Status) << ")";
        throw e;
    }
}

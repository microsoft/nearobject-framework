
#include <windows/devices/uwb/simulator/UwbDeviceSimulatorConnector.hxx>

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

UwbDeviceSimulatorConnector::UwbDeviceSimulatorConnector(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{}

std::future<UwbSimulatorCapabilities>
UwbDeviceSimulatorConnector::GetCapabilites()
{
    std::promise<UwbSimulatorCapabilities> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES

    return resultFuture;
}

std::future<UwbSimulatorTriggerSessionEventResult>
UwbDeviceSimulatorConnector::TriggerSessionEvent([[maybe_unused]] const UwbSimulatorTriggerSessionEventArgs& triggerSessionEventArgs)
{
    std::promise<UwbSimulatorTriggerSessionEventResult> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_DEVICE_SIM_TRIGGER_SESSION_EVENT

    return resultFuture;
}

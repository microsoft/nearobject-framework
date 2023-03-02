
#include <windows/devices/uwb/simulator/UwbDeviceSimulator.hxx>

#include <UwbSimulatorDdiGlue.h>

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
    UwbDevice::Initialize();
    m_handleDriver = UwbDevice::DriverHandle();
    return true;
}

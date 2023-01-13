
#include <windows/devices/uwb/simulator/UwbDeviceSimulator.hxx>

#include <UwbSimulatorDdiGlue.h>

using namespace windows::devices::uwb::simulator;

UwbDeviceSimulator::UwbDeviceSimulator(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{}

const std::string&
UwbDeviceSimulator::DeviceName() const noexcept
{
    return m_deviceName;
}

void
UwbDeviceSimulator::Initialize()
{
    wil::unique_hfile handleDriver(CreateFileA(
        m_deviceName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        nullptr));

    m_handleDriver = std::move(handleDriver);
}

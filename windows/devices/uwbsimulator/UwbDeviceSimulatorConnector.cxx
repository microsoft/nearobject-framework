
#include <windows/devices/uwb/simulator/UwbDeviceSimulatorConnector.hxx>

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

UwbDeviceSimulatorConnector::UwbDeviceSimulatorConnector(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{}

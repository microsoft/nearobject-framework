
#include <windows/devices/uwb/simulator/UwbSessionSimulator.hxx>

using namespace windows::devices::uwb::simulator;

UwbSessionSimulator::UwbSessionSimulator(std::shared_ptr<UwbDeviceConnector> uwbDeviceConnector, std::shared_ptr<UwbDeviceSimulatorConnector> uwbDeviceSimulatorConnector) :
    UwbSession(std::move(uwbDeviceConnector)),
    m_uwbDeviceSimulatorConnector(std::move(uwbDeviceSimulatorConnector))
{}

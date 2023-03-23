
#include <windows/devices/uwb/simulator/UwbSessionSimulator.hxx>

using namespace windows::devices::uwb::simulator;

UwbSessionSimulator::UwbSessionSimulator(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, std::shared_ptr<IUwbSessionDdiConnector> uwbDeviceConnector, std::shared_ptr<UwbDeviceSimulatorConnector> uwbDeviceSimulatorConnector) :
    UwbSession(std::move(callbacks), std::move(uwbDeviceConnector)),
    m_uwbDeviceSimulatorConnector(std::move(uwbDeviceSimulatorConnector))
{}

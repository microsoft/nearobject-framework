
#include <windows/devices/uwb/simulator/UwbSessionSimulator.hxx>

using namespace windows::devices::uwb::simulator;

UwbSessionSimulator::UwbSessionSimulator(uint32_t sessionId, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, std::shared_ptr<UwbDeviceConnector> uwbDeviceConnector, std::shared_ptr<UwbDeviceSimulatorConnector> uwbDeviceSimulatorConnector) :
    UwbSession(sessionId, std::move(callbacks), std::move(uwbDeviceConnector)),
    m_uwbDeviceSimulatorConnector(std::move(uwbDeviceSimulatorConnector))
{}

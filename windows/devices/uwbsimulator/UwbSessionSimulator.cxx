
#include <windows/devices/uwb/simulator/UwbSessionSimulator.hxx>

using namespace windows::devices::uwb::simulator;

UwbSessionSimulator::UwbSessionSimulator(uint32_t sessionId, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, UwbDevice* uwbDevice, std::shared_ptr<UwbDeviceSimulatorConnector> uwbDeviceSimulatorConnector) :
    UwbSession(sessionId, std::move(callbacks), uwbDevice),
    m_uwbDeviceSimulatorConnector(std::move(uwbDeviceSimulatorConnector))
{}

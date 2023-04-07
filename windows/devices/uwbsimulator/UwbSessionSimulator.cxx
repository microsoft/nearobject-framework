
#include <windows/devices/uwb/simulator/UwbSessionSimulator.hxx>

using namespace windows::devices::uwb::simulator;

UwbSessionSimulator::UwbSessionSimulator(uint32_t sessionId, std::weak_ptr<::uwb::UwbDevice> device, std::shared_ptr<IUwbSessionDdiConnector> uwbSessionConnector, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, std::shared_ptr<UwbDeviceSimulatorConnector> uwbDeviceSimulatorConnector) :
    UwbSession(sessionId, std::move(device), std::move(uwbSessionConnector), std::move(callbacks)),
    m_uwbDeviceSimulatorConnector(std::move(uwbDeviceSimulatorConnector))
{}

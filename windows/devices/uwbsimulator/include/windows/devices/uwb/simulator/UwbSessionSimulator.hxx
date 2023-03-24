
#ifndef UWB_SESSION_SIMULATOR_HXX
#define UWB_SESSION_SIMULATOR_HXX

#include <memory>

#include <windows.h>

#include <wil/resource.h>

#include <uwb/UwbSessionEventCallbacks.hxx>
#include <windows/devices/uwb/UwbSession.hxx>
#include <windows/devices/uwb/simulator/UwbDeviceSimulatorConnector.hxx>

namespace windows::devices::uwb::simulator
{
class UwbSessionSimulator :
    public windows::devices::uwb::UwbSession
{
public:
    UwbSessionSimulator(uint32_t sessionId, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, std::shared_ptr<UwbDeviceConnector> uwbDeviceConnector, std::shared_ptr<UwbDeviceSimulatorConnector> uwbDeviceSimulatorConnector);

    virtual ~UwbSessionSimulator() = default;

private:
    std::shared_ptr<UwbDeviceSimulatorConnector> m_uwbDeviceSimulatorConnector;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SESSION_SIMULATOR_HXX

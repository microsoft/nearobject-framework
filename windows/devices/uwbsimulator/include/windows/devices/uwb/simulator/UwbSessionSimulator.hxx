
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
/**
 * @brief Simulator driver session.
 *
 * This class is used to represent simulator driver specific session context.
 */
class UwbSessionSimulator :
    public windows::devices::uwb::UwbSession
{
public:
    /**
     * @brief Construct a new UwbSessionSimulator object.
     *
     * @param sessionId The session identifier.
     * @param device Reference to the parent device.
     * @param uwbSessionConnector The connector used to communicate with the uwb device.
     * @param callbacks The event callback instance.
     * @param uwbDeviceSimulatorConnector The connector used to communicate with the simulator device.
     */
    UwbSessionSimulator(uint32_t sessionId, std::weak_ptr<::uwb::UwbDevice> device, std::shared_ptr<IUwbSessionDdiConnector> uwbSessionConnector, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, std::shared_ptr<UwbDeviceSimulatorConnector> uwbDeviceSimulatorConnector);

    virtual ~UwbSessionSimulator() = default;

private:
    std::shared_ptr<UwbDeviceSimulatorConnector> m_uwbDeviceSimulatorConnector;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SESSION_SIMULATOR_HXX

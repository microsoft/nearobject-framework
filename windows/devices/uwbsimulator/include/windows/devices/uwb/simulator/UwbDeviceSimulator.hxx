
#ifndef UWB_DEVICE_SIMULATOR_HXX
#define UWB_DEVICE_SIMULATOR_HXX

#include <cstdint>
#include <memory>
#include <string>

// NB: This must come before any other Windows include
#include <windows.h>

#include <UwbSimulatorDdiGlue.h>

#include <cfgmgr32.h>
#include <wil/resource.h>
#include <windows/devices/DeviceResource.hxx>
#include <windows/devices/uwb/IUwbDeviceDdi.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
#include <windows/devices/uwb/simulator/UwbDeviceSimulatorConnector.hxx>

namespace windows::devices::uwb::simulator
{
class UwbDeviceSimulator :
    public windows::devices::uwb::UwbDevice
{
public:
    /**
     * @brief Construct a new UwbDeviceSimulator object.
     *
     * @param deviceName The device name (path).
     */
    explicit UwbDeviceSimulator(std::string deviceName);

    /**
     * @brief Get the name of this device.
     *
     * @return const std::string&
     */
    const std::string&
    DeviceName() const noexcept;

    /**
     * @brief Initialize the simulator device.
     *
     * @return true
     * @return false
     */
    bool
    Initialize();

    /**
     * @brief Obtain the capabilities of the simulator driver.
     *
     * Internally, this invokes IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES on the
     * primary driver handle.
     *
     * @return UwbSimulatorCapabilities
     */
    UwbSimulatorCapabilities
    GetSimulatorCapabilities();

    /**
     * @brief Trigger an event for a session. 
     * 
     * @param triggerSessionEventArgs 
     * @return UwbSimulatorTriggerSessionEventResult 
     */
    UwbSimulatorTriggerSessionEventResult
    TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs& triggerSessionEventArgs);

private:
    virtual std::shared_ptr<::uwb::UwbSession>
    CreateSessionImpl(uint32_t sessionId, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks) override;

private:
    const std::string m_deviceName;
    std::shared_ptr<UwbDeviceSimulatorConnector> m_uwbDeviceSimulatorConnector;
};

} // namespace windows::devices::uwb::simulator

#endif // UWB_DEVICE_SIMULATOR_HXX

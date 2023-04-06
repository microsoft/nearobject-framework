
#ifndef UWB_DEVICE_SIMULATOR_CONNECTOR_HXX
#define UWB_DEVICE_SIMULATOR_CONNECTOR_HXX

#include <memory>
#include <string>

#include <windows.h>

#include <wil/resource.h>

#include <windows/devices/uwb/simulator/IUwbDeviceSimulatorDdi.hxx>

namespace windows::devices::uwb::simulator
{
/**
 * @brief Provides access to the DDI exposed by GUID_DEVINTERFACE_UWB_SIMULATOR
 * described in UwbSimulatorDdi.h.
 */
class UwbDeviceSimulatorConnector :
    public IUwbDeviceSimulatorDdi
{
public:
    /**
     * @brief Construct a new Uwb Device Simulator Connector object.
     *
     * @param deviceName
     */
    explicit UwbDeviceSimulatorConnector(std::string deviceName);

public:
    // IUwbDeviceSimulatorDdi
    virtual std::future<UwbSimulatorCapabilities>
    GetCapabilites() override;

    virtual std::future<UwbSimulatorTriggerSessionEventResult>
    TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs& triggerSessionEventArgs) override;

private:
    wil::shared_hfile m_driverHandle;
    std::string m_deviceName{};
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_DEVICE_SIMULATOR_CONNECTOR_HXX

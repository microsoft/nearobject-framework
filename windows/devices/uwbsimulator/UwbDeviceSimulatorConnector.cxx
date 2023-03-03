
#include <ios>
#include <iomanip>

#include <plog/Log.h>
#include <wil/result.h>

#include <ioapiset.h>

#include <windows/devices/DeviceHandle.hxx>
#include <windows/devices/uwb/simulator/UwbDeviceSimulatorConnector.hxx>

using namespace windows::devices;
using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

UwbDeviceSimulatorConnector::UwbDeviceSimulatorConnector(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{}

std::future<UwbSimulatorCapabilities>
UwbDeviceSimulatorConnector::GetCapabilites()
{
    std::promise<UwbSimulatorCapabilities> resultPromise;
    auto resultFuture = resultPromise.get_future();

    wil::unique_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << std::showbase << std::hex << hr;
        return resultFuture;
    }

    UwbSimulatorCapabilities simulatorCapabilities{};
    DWORD bytesRequired = sizeof simulatorCapabilities;
    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES, nullptr, 0, &simulatorCapabilities, sizeof simulatorCapabilities, nullptr, nullptr);
    if (!ioResult) {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES, hr=" << std::showbase << std::hex << hr;
    }

    resultPromise.set_value(std::move(simulatorCapabilities));

    return resultFuture;
}

std::future<UwbSimulatorTriggerSessionEventResult>
UwbDeviceSimulatorConnector::TriggerSessionEvent([[maybe_unused]] const UwbSimulatorTriggerSessionEventArgs& triggerSessionEventArgs)
{
    std::promise<UwbSimulatorTriggerSessionEventResult> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_DEVICE_SIM_TRIGGER_SESSION_EVENT

    return resultFuture;
}

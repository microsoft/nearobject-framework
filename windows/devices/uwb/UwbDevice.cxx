
#include "UwbDevice.hxx"

using namespace windows::devices;

UwbDevice::UwbDevice(std::wstring deviceName) :
    m_deviceName(std::move(deviceName))
{}

const std::wstring&
UwbDevice::DeviceName() const noexcept
{
    return m_deviceName;
}

void
UwbDevice::Initialize()
{
    // TODO: wil::unique_hfile handleDriver(CreateFile(m_deviceName))
    // TODO: call CM_Register_Notification(handleDriver, filterType=CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE)
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVE -> close handleDriver since device removal is requested
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVEFAILED -> query removal failed
}

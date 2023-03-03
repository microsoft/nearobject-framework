
#include <windows/devices/DeviceHandle.hxx>

HRESULT
windows::devices::OpenDriverHandleShared(wil::shared_hfile &driverHandle, const char *deviceName, bool isOverlapped)
{
    return OpenDriverHandle<wil::shared_hfile>(driverHandle, deviceName, isOverlapped);
}

HRESULT
windows::devices::OpenDriverHandle(wil::unique_hfile &driverHandle, const char *deviceName, bool isOverlapped)
{
    return OpenDriverHandle<wil::unique_hfile>(driverHandle, deviceName, isOverlapped);
}

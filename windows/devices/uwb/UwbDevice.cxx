
#include <UwbCxLrpDeviceGlue.h>

#include <windows/uwb/UwbDevice.hxx>
#include <windows/uwb/UwbSession.hxx>

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
    wil::unique_hfile handleDriver(CreateFile(
        m_deviceName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        nullptr));

    // TODO: wil::unique_hfile handleDriver(CreateFile(m_deviceName))
    // TODO: call CM_Register_Notification(handleDriver, filterType=CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE)
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVE -> close handleDriver since device removal is requested
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVEFAILED -> query removal failed

    m_handleDriver = std::move(handleDriver);
}

std::unique_ptr<uwb::UwbSession>
UwbDevice::CreateSession(uint32_t sessionId)
{
    // Create a duplicate handle to the driver for use by the session.
    wil::unique_hfile handleDriverForSession;
    if (!DuplicateHandle(GetCurrentProcess(), m_handleDriver.get(), GetCurrentProcess(), &handleDriverForSession, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
        return nullptr;
    }

    // Populate the session initialization command argument.
    UWB_SESSION_INIT sessionInit;
    sessionInit.sessionId = sessionId;
    sessionInit.sessionType = UWB_SESSION_TYPE_RANGING_SESSION;

    // Request a new session from the driver.
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SESSION_INIT, &sessionInit, sizeof sessionInit, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        return nullptr;
    }

    return std::make_unique<UwbSession>(sessionId, std::move(handleDriverForSession));
}

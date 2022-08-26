
#include <windows/devices/DeviceEnumerator.hxx>

#include <cfgmgr32.h>

using namespace windows::devices;

/* static */
std::vector<std::wstring>
DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(const GUID& deviceInterfaceClassGuidTarget) noexcept
{
    // Have to copy the GUID since the API oddly requires a non-const reference,
    // even though its contract indicates the GUID is an input-only argument :(
    GUID deviceClassInterfaceGuid = deviceInterfaceClassGuidTarget;
    ULONG deviceInterfaceNamesBufferSize = 0;

    // Determine the size of the list needed.
    CONFIGRET configRet = CM_Get_Device_Interface_List_Size(
        &deviceInterfaceNamesBufferSize, 
        &deviceClassInterfaceGuid,
        nullptr, 
        CM_GET_DEVICE_INTERFACE_LIST_PRESENT);
    if (configRet != CR_SUCCESS) {
        // TODO: ???
        return {};
    } else if (deviceInterfaceNamesBufferSize == 0) {
        return {};
    }

    std::vector<wchar_t> deviceInterfaceNamesBuffer(deviceInterfaceNamesBufferSize);
    
    // Query for the actual list.
    configRet = CM_Get_Device_Interface_List(
        &deviceClassInterfaceGuid, 
        nullptr, 
        std::data(deviceInterfaceNamesBuffer),
        deviceInterfaceNamesBufferSize,
        CM_GET_DEVICE_INTERFACE_LIST_PRESENT);
    if (configRet != CR_SUCCESS) {
        // TODO: ???
        return {};
    }

    // Pull out individual strings from the double-null terminated list returned above.
    std::vector<std::wstring> deviceInterfaceNames{};
    for (auto deviceInterfaceName = std::data(deviceInterfaceNamesBuffer); *deviceInterfaceName != L'\0'; ) {
        deviceInterfaceNames.emplace_back(deviceInterfaceName);
        deviceInterfaceName += deviceInterfaceNames.back().size() + 1;
    }

    return deviceInterfaceNames;
}

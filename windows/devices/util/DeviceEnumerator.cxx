
#include <filesystem>

#include <notstd/guid.hxx>
#include <windows/devices/DeviceEnumerator.hxx>

#include <cfgmgr32.h>

#include <plog/Log.h>

using namespace windows::devices;

/* static */
std::vector<std::string>
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
        PLOG_ERROR << "CM_Get_Device_Interface_List_Size failed";
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
        PLOG_ERROR << "CM_Get_Device_Interface_List failed";
        return {};
    }


    // Pull out individual strings from the double-null terminated list returned above.
    std::vector<std::string> deviceInterfaceNames{};
    for (auto deviceInterfaceName = std::data(deviceInterfaceNamesBuffer); *deviceInterfaceName != L'\0';) {
        std::filesystem::path deviceInterfaceNamePath{ deviceInterfaceName };
        deviceInterfaceNames.emplace_back(deviceInterfaceNamePath.string());
        deviceInterfaceName += deviceInterfaceNames.back().size() + 1;
    }

    return deviceInterfaceNames;
}

/* static */
std::vector<std::string>
DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(std::string_view deviceInterfaceClassString) noexcept
{
    auto deviceClassInterfaceGuid = notstd::GuidFromStringView(deviceInterfaceClassString);
    if (!deviceClassInterfaceGuid.has_value()) {
        PLOG_DEBUG << "empty guid";
        return {};
    }

    return GetDeviceInterfaceClassInstanceNames(deviceClassInterfaceGuid.value());
}

#include <windows/devices/uwb/UwbCxAdapter.hxx>

#include <windows/devices/uwb/UwbAppConfiguration.hxx>

using namespace windows::devices::uwb::detail;
using namespace windows::devices;

std::unique_ptr<uint8_t[]>
windows::devices::uwb::detail::GenerateUwbSetAppConfigParameterDdi(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData)
{
    UwbSetAppConfigurationParametersBuilder builder{ uwbSessionData.sessionId };

    auto role = uwbSessionData.uwbConfiguration.GetDeviceRole();
    if (role.has_value()) {
        UwbAppConfigurationParameter devRole{ *role, service_ddi_map[::uwb::protocol::fira::UwbConfiguration::ParameterTag::DeviceRole] };
        builder.Push(std::move(devRole));
    }

    auto multiNodeMode = uwbSessionData.uwbConfiguration.GetMultiNodeMode();
    if (multiNodeMode.has_value()) {
        builder.Push(std::move(UwbAppConfigurationParameter{ *multiNodeMode, service_ddi_map[::uwb::protocol::fira::UwbConfiguration::ParameterTag::MultiNodeMode] }));
    }

    // TODO NUMBER_OF_CONTROLEES
    // TODO DEVICE_MAC_ADDRESS
    // TODO DST_MAC_ADDRESS
    // TODO DEVICE_TYPE

    return builder.Publish();
}

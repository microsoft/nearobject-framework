
#include <algorithm>

#include <windows/devices/uwb/UwbCxAdapter.hxx>

#include <windows/devices/uwb/UwbAppConfiguration.hxx>

using namespace windows::devices;

UwbSetAppConfigurationParameters
windows::devices::uwb::GenerateUwbSetAppConfigParameterDdi(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData)
{
    std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> parameters;

    // TODO NUMBER_OF_CONTROLEES
    // TODO DEVICE_MAC_ADDRESS
    // TODO DST_MAC_ADDRESS
    // TODO DEVICE_TYPE
    const auto sessionUwbMap = uwbSessionData.uwbConfiguration.GetValueMap();

    for (const auto& [parameterTag, parameterValueVariant] : sessionUwbMap) {
        std::visit([&](auto&& parameterValue) {
            using ParameterValueT = std::decay_t<decltype(parameterValue)>;
            if constexpr (std::is_enum_v<ParameterValueT>) {
                parameters.push_back(std::make_shared<UwbAppConfigurationParameter<std::underlying_type_t<ParameterValueT>>>(notstd::to_underlying(parameterValue), parameterTag));
            } else if constexpr (std::is_integral_v<ParameterValueT>) {
                parameters.push_back(std::make_shared<UwbAppConfigurationParameter<ParameterValueT>>(parameterValue, parameterTag));
            } else if constexpr (std::is_same_v<ParameterValueT, ::uwb::protocol::fira::RangingMethod>) {
                parameters.push_back(std::make_shared<UwbAppConfigurationParameter<uint8_t>>(parameterValue.ToByte(), parameterTag));
            } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<::uwb::protocol::fira::ResultReportConfiguration>>) {
                uint8_t result = 0;
                for (const auto& config : parameterValue) {
                    result |= notstd::to_underlying(config);
                }
                parameters.push_back(std::make_shared<UwbAppConfigurationParameter<uint8_t>>(result, parameterTag));
            } else if constexpr (std::is_same_v<ParameterValueT, ::uwb::UwbMacAddress>) {
                auto value = parameterValue.GetValue();
                // Convert the underlying raw value (std::span<uint8_t>) to a standard layout type.
                switch (value.size()) {
                case ::uwb::UwbMacAddressLength::Short: {
                    std::array<uint8_t, ::uwb::UwbMacAddressLength::Short> valueFixed{};
                    std::copy(value.begin(), value.end(), valueFixed.begin());
                    parameters.push_back(std::make_shared<UwbAppConfigurationParameter<decltype(valueFixed)>>(valueFixed, parameterTag));
                    break;
                }
                case ::uwb::UwbMacAddressLength::Extended: {
                    std::array<uint8_t, ::uwb::UwbMacAddressLength::Extended> valueFixed{};
                    std::copy(value.begin(), value.end(), valueFixed.begin());
                    parameters.push_back(std::make_shared<UwbAppConfigurationParameter<decltype(valueFixed)>>(valueFixed, parameterTag));
                    break;
                }
                default: {
                    break;
                }
                } // switch (value.size())
            } else {
                // TODO: should not ever get here, log?
            }
        },
            parameterValueVariant);
    }

    return UwbSetAppConfigurationParameters{ parameters };
}

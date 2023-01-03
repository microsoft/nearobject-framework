#include <windows/devices/uwb/UwbCxAdapter.hxx>

#include <windows/devices/uwb/UwbAppConfiguration.hxx>

using namespace windows::devices::uwb::detail;
using namespace windows::devices;

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

std::unique_ptr<uint8_t[]>
windows::devices::uwb::detail::GenerateUwbSetAppConfigParameterDdi(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData)
{
    UwbSetAppConfigurationParametersBuilder builder{ uwbSessionData.sessionId };

    auto role = uwbSessionData.uwbConfiguration.GetDeviceRole();
    if (role.has_value()) {
        UwbAppConfigurationParameter devRole{ *role, service_ddi_map[::uwb::protocol::fira::UwbConfiguration::ParameterTag::DeviceRole] };
        builder.Push(std::move(devRole));
    }

    // TODO NUMBER_OF_CONTROLEES
    // TODO DEVICE_MAC_ADDRESS
    // TODO DST_MAC_ADDRESS
    // TODO DEVICE_TYPE
    const auto sessionUwbMap = uwbSessionData.uwbConfiguration.GetValueMap();

    for (const auto [ptag, value] : sessionUwbMap) {
        std::visit(overloaded{
                       [&builder, ptag](bool arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](uint8_t arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](uint16_t arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint16_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](uint32_t arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint32_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::Channel arg) {
                           uint8_t data = 9;
                           switch (arg) {
                           case (::uwb::protocol::fira::Channel::C5): {
                               data = 5;
                               break;
                           }
                           case (::uwb::protocol::fira::Channel::C6): {
                               data = 6;
                               break;
                           }
                           case (::uwb::protocol::fira::Channel::C8): {
                               data = 8;
                               break;
                           }
                           case (::uwb::protocol::fira::Channel::C9): {
                               data = 9;
                               break;
                           }
                           case (::uwb::protocol::fira::Channel::C10): {
                               data = 10;
                               break;
                           }
                           case (::uwb::protocol::fira::Channel::C12): {
                               data = 12;
                               break;
                           }
                           case (::uwb::protocol::fira::Channel::C13): {
                               data = 13;
                               break;
                           }
                           case (::uwb::protocol::fira::Channel::C14): {
                               data = 14;
                               break;
                           }
                           }
                           builder.Push(std::move(UwbAppConfigurationParameter{ data, service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::ConvolutionalCodeConstraintLength arg) {
                           // TODO
                       },
                       [&builder, ptag](::uwb::protocol::fira::DeviceRole arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::MultiNodeMode arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::PrfMode arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::RangingMethod arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg.ToByte()), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::RangingMode arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::SchedulingMode arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::StsConfiguration arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::StsPacketConfiguration arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::UwbMacAddress arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ arg.GetValue(), service_ddi_map[ptag] }));
                           //    if (ptag == ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ControllerMacAddress) {
                           //        builder.Push(std::move(UwbAppConfigurationParameter{ arg.GetValue(), service_ddi_map[ptag] }));
                           //    } else if(ptag == ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ControleeShortMacAddress){
                           //        builder.Push(std::move(UwbAppConfigurationParameter{ arg.GetValue(), service_ddi_map[ptag] }));
                           //    }
                       },
                       [&builder, ptag](::uwb::UwbMacAddressFcsType arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map[ptag] }));
                       },
                       [&builder, ptag](::uwb::UwbMacAddressType arg) {
                       },
                       [&builder, ptag](std::unordered_set<::uwb::protocol::fira::ResultReportConfiguration> arg) {
                           uint8_t result = 0;
                           for (auto config : arg) {
                               switch (config) {
                               case ::uwb::protocol::fira::ResultReportConfiguration::TofReport: {
                                   result |= 0x1;
                                   break;
                               }
                               case ::uwb::protocol::fira::ResultReportConfiguration::AoAAzimuthReport: {
                                   result |= 0x2;
                                   break;
                               }
                               case ::uwb::protocol::fira::ResultReportConfiguration::AoAElevationReport: {
                                   result |= 0x4;
                                   break;
                               }
                               default: {
                                   result |= 0x8;
                                   break;
                               }
                               }
                           }
                           builder.Push(std::move(UwbAppConfigurationParameter{ result, service_ddi_map[ptag] }));
                       } },
            value);
    }

    return builder.Publish();
}

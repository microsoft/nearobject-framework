#include <windows/devices/uwb/UwbCxAdapter.hxx>

#include <windows/devices/uwb/UwbAppConfiguration.hxx>

namespace windows::devices::uwb::detail
{
const std::unordered_map<
    ::uwb::protocol::fira::UwbConfiguration::ParameterTag,
    UWB_APP_CONFIG_PARAM_TYPE>
    service_ddi_map{
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::DeviceRole, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::RangingMethod, UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::StsConfig, UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MultiNodeMode, UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::RangingTimeStruct, UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ScheduledMode, UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::HoppingMode, UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::BlockStriding, UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH }, // TODO the stride length needs to be decided somewhere
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::UwbInitiationTime, UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ChannelNumber, UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::RFrameConfig, UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::CcConstraintLength, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE }, // TODO ccclength is related but not directly the psdu data rate
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::PrfMode, UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::Sp0PhySetNumber, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE }, // TODO related but not directly the psdu data rate
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::Sp1PhySetNumber, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE }, // TODO related but not directly the psdu data rate
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::Sp3PhySetNumber, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE }, // TODO related but not directly the psdu data rate
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::PreableCodeIndex, UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ResultReportConfig, UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MacAddressMode, UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ControleeShortMacAddress, UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ControllerMacAddress, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS }, // TODO this field only applies when the host is the controller
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::SlotsPerRr, UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MaxContentionPhaseLength, UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR }, // TODO figure out this field
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::SlotDuration, UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::RangingInterval, UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::KeyRotationRate, UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MacFcsType, UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MaxRrRetry, UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY },
    };
} // namespace windows::devices::uwb::detail

using namespace windows::devices::uwb::detail;
using namespace windows::devices;

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

std::unique_ptr<uint8_t[]>
windows::devices::uwb::GenerateUwbSetAppConfigParameterDdi(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData)
{
    UwbSetAppConfigurationParametersBuilder builder{ uwbSessionData.sessionId };

    // TODO NUMBER_OF_CONTROLEES
    // TODO DEVICE_MAC_ADDRESS
    // TODO DST_MAC_ADDRESS
    // TODO DEVICE_TYPE
    const auto sessionUwbMap = uwbSessionData.uwbConfiguration.GetValueMap();

    for (const auto [ptag, value] : sessionUwbMap) {
        std::visit(overloaded{
                       [&builder, ptag](bool arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](uint8_t arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](uint16_t arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint16_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](uint32_t arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint32_t(arg), service_ddi_map.at(ptag) }));
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
                           builder.Push(std::move(UwbAppConfigurationParameter{ data, service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::ConvolutionalCodeConstraintLength arg) {
                           // TODO
                       },
                       [&builder, ptag](::uwb::protocol::fira::DeviceRole arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::MultiNodeMode arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::PrfMode arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::RangingMethod arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg.ToByte()), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::RangingMode arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::SchedulingMode arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::StsConfiguration arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::protocol::fira::StsPacketConfiguration arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
                       },
                       [&builder, ptag](::uwb::UwbMacAddress arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ arg.GetValue(), service_ddi_map.at(ptag) }));
                           //    if (ptag == ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ControllerMacAddress) {
                           //        builder.Push(std::move(UwbAppConfigurationParameter{ arg.GetValue(), service_ddi_map.at(ptag) }));
                           //    } else if(ptag == ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ControleeShortMacAddress){
                           //        builder.Push(std::move(UwbAppConfigurationParameter{ arg.GetValue(), service_ddi_map.at(ptag) }));
                           //    }
                       },
                       [&builder, ptag](::uwb::UwbMacAddressFcsType arg) {
                           builder.Push(std::move(UwbAppConfigurationParameter{ uint8_t(arg), service_ddi_map.at(ptag) }));
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
                           builder.Push(std::move(UwbAppConfigurationParameter{ result, service_ddi_map.at(ptag) }));
                       } },
            value);
    }

    return builder.Publish();
}

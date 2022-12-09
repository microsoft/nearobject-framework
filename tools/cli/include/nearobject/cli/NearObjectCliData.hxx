
#ifndef NEAR_OBJECT_CLI_DATA_HXX
#define NEAR_OBJECT_CLI_DATA_HXX

#include <map>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace nearobject::cli
{
/**
 * @brief Base class for data parsed by NearObjectCli.
 */
struct NearObjectCliData
{
    std::unordered_map<std::string, uwb::protocol::fira::DeviceRole> DeviceRoleMap;
    std::unordered_map<std::string, uwb::protocol::fira::RangingMethod> RangingMethodMap;
    std::unordered_map<std::string, uwb::protocol::fira::MeasurementReportMode> MeasurementReportModeMap;
    std::unordered_map<std::string, uwb::protocol::fira::RangingConfiguration> RangingConfigurationMap;
    std::unordered_map<std::string, uwb::protocol::fira::StsConfiguration> StsConfigurationMap;
    std::unordered_map<std::string, uwb::protocol::fira::MultiNodeMode> MultiNodeModeMap;
    std::unordered_map<std::string, uwb::protocol::fira::RangingMode> RangingModeMap;
    std::unordered_map<std::string, uwb::protocol::fira::SchedulingMode> SchedulingModeMap;
    std::unordered_map<std::string, uwb::protocol::fira::Channel> ChannelMap;
    std::unordered_map<std::string, uwb::protocol::fira::StsPacketConfiguration> StsPacketConfigurationMap;
    std::unordered_map<std::string, uwb::protocol::fira::ConvolutionalCodeConstraintLength> ConvolutionalCodeConstraintLengthMap;
    std::unordered_map<std::string, uwb::protocol::fira::PrfMode> PrfModeMap;
    std::unordered_map<std::string, uwb::UwbMacAddressFcsType> UwbMacAddressFcsTypeMap;
    std::unordered_map<std::string, uwb::protocol::fira::ResultReportConfiguration> ResultReportConfigurationMap;

    std::string MacVersionString;
    std::string PhyVersionString;

    bool HostIsController{ false };
    uwb::protocol::fira::UwbSessionData SessionData{};
};

} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX

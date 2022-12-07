
#ifndef NEAR_OBJECT_CLI_DATA_HXX
#define NEAR_OBJECT_CLI_DATA_HXX

#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <map>

namespace nearobject::cli
{
/**
 * @brief Base class for data parsed by NearObjectCli.
 */
struct NearObjectCliData
{
    std::map<std::string,uwb::protocol::fira::DeviceRole> DeviceRoleMap;
    std::map<std::string,uwb::protocol::fira::RangingConfiguration> RangingConfigurationMap;
    std::map<std::string,uwb::protocol::fira::StsConfiguration> StsConfigurationMap;
    std::map<std::string,uwb::protocol::fira::MultiNodeMode> MultiNodeModeMap;
    std::map<std::string,uwb::protocol::fira::RangingMode> RangingModeMap;
    std::map<std::string,uwb::protocol::fira::SchedulingMode> SchedulingModeMap;
    std::map<std::string,uwb::protocol::fira::Channel> ChannelMap;
    std::map<std::string,uwb::protocol::fira::StsPacketConfiguration> StsPacketConfigurationMap;
    std::map<std::string,uwb::protocol::fira::ConvolutionalCodeConstraintLength> ConvolutionalCodeConstraintLengthMap;
    std::map<std::string,uwb::protocol::fira::PrfMode> PrfModeMap;

    std::string defaultFile {"default.json"};
    bool hostIsController {false};
    uwb::protocol::fira::UwbConfiguration defaultConfiguration {};
};



} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX

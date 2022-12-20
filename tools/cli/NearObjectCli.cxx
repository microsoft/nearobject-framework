
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <magic_enum.hpp>
#include <nearobject/cli/NearObjectCli.hxx>
#include <notstd/tostring.hxx>

using namespace nearobject::cli;
using namespace strings::ostream_operators;

NearObjectCli::NearObjectCli(std::shared_ptr<NearObjectCliData> cliData, std::shared_ptr<NearObjectCliHandler> cliHandler) :
    m_cliData(cliData),
    m_cliHandler(cliHandler)
{
    m_cliApp = CreateParser();

    if (!m_cliApp) {
        throw std::runtime_error("failed to create command line parser");
    }
}

std::shared_ptr<NearObjectCliData>
NearObjectCli::GetData() const noexcept
{
    return m_cliData;
}

int
NearObjectCli::Parse(int argc, char* argv[]) noexcept
{
    try {
        m_cliApp->parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return m_cliApp->exit(e);
    }

    return 0;
}

CLI::App&
NearObjectCli::GetParser()
{
    return *m_cliApp;
}

CLI::App&
NearObjectCli::GetUwbApp() noexcept
{
    return *m_uwbApp;
}

CLI::App&
NearObjectCli::GetRangeApp() noexcept
{
    return *m_rangeApp;
}

CLI::App&
NearObjectCli::GetRangeStartApp() noexcept
{
    return *m_rangeStartApp;
}

CLI::App&
NearObjectCli::GetRangeStopApp() noexcept
{
    return *m_rangeStopApp;
}

std::shared_ptr<uwb::UwbDevice>
NearObjectCli::GetUwbDevice() noexcept
{
    if (m_uwbDevice == nullptr) {
        m_uwbDevice = m_cliHandler->ResolveUwbDevice(*m_cliData);
    }

    return m_uwbDevice;
}

std::unique_ptr<CLI::App>
NearObjectCli::CreateParser() noexcept
{
    // top-level command
    auto app = std::make_unique<CLI::App>("nocli", "A command line tool to assist with all things nearobject");
    app->require_subcommand();

    // sub-commands
    m_uwbApp = AddSubcommandUwb(app.get());

    return app;
}

namespace detail
{
/**
 * @brief Helper function to populate the string-enum mapping
 *
 * @tparam EnumType
 * @return std::map<std::string, EnumType>
 */
template <typename EnumType>
const std::unordered_map<std::string, EnumType>
populateMap() noexcept
{
    const auto reverseMap = magic_enum::enum_entries<EnumType>();
    std::vector<std::pair<std::string, EnumType>> destVector{ reverseMap.size() };
    std::transform(std::cbegin(reverseMap),
        std::cend(reverseMap),
        std::begin(destVector),
        [](const auto& input) {
            return std::pair{ std::string{ input.second }, input.first };
        });
    return { std::cbegin(destVector), std::cend(destVector) };
}

/**
 * @brief Helper function to return a tuple containing the string
 * representations of an enum type and the value.
 *
 * @tparam EnumT The type of the enumeration.
 * @param value The enumeration value.
 * @return std::tuple<std::string_view, std::string_view>
 */
template <typename EnumT>
std::tuple<std::string_view, std::string_view>
GetEnumTypeAndValue(const std::optional<EnumT>& value)
{
    return std::make_tuple(magic_enum::enum_type_name<EnumT>(), magic_enum::enum_name(*value));
}

const std::unordered_map<std::string, uwb::protocol::fira::DeviceRole> DeviceRoleMap = populateMap<uwb::protocol::fira::DeviceRole>();
const std::unordered_map<std::string, uwb::protocol::fira::RangingMethod> RangingMethodMap = populateMap<uwb::protocol::fira::RangingMethod>();
const std::unordered_map<std::string, uwb::protocol::fira::MeasurementReportMode> MeasurementReportModeMap = populateMap<uwb::protocol::fira::MeasurementReportMode>();
const std::unordered_map<std::string, uwb::protocol::fira::StsConfiguration> StsConfigurationMap = populateMap<uwb::protocol::fira::StsConfiguration>();
const std::unordered_map<std::string, uwb::protocol::fira::MultiNodeMode> MultiNodeModeMap = populateMap<uwb::protocol::fira::MultiNodeMode>();
const std::unordered_map<std::string, uwb::protocol::fira::RangingMode> RangingModeMap = populateMap<uwb::protocol::fira::RangingMode>();
const std::unordered_map<std::string, uwb::protocol::fira::SchedulingMode> SchedulingModeMap = populateMap<uwb::protocol::fira::SchedulingMode>();
const std::unordered_map<std::string, uwb::protocol::fira::Channel> ChannelMap = populateMap<uwb::protocol::fira::Channel>();
const std::unordered_map<std::string, uwb::protocol::fira::StsPacketConfiguration> StsPacketConfigurationMap = populateMap<uwb::protocol::fira::StsPacketConfiguration>();
const std::unordered_map<std::string, uwb::protocol::fira::ConvolutionalCodeConstraintLength> ConvolutionalCodeConstraintLengthMap = populateMap<uwb::protocol::fira::ConvolutionalCodeConstraintLength>();
const std::unordered_map<std::string, uwb::protocol::fira::PrfMode> PrfModeMap = populateMap<uwb::protocol::fira::PrfMode>();
const std::unordered_map<std::string, uwb::UwbMacAddressFcsType> UwbMacAddressFcsTypeMap = populateMap<uwb::UwbMacAddressFcsType>();
const std::unordered_map<std::string, uwb::protocol::fira::ResultReportConfiguration> ResultReportConfigurationMap = populateMap<uwb::protocol::fira::ResultReportConfiguration>();
} // namespace detail

CLI::App*
NearObjectCli::AddSubcommandUwb(CLI::App* parent)
{
    // top-level command
    auto uwbApp = parent->add_subcommand("uwb", "commands related to uwb")->require_subcommand()->fallthrough();

    // sub-commands
    m_rangeApp = AddSubcommandUwbRange(uwbApp);

    return uwbApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRange(CLI::App* parent)
{
    // top-level command
    auto rangeApp = parent->add_subcommand("range", "commands related to ranging")->require_subcommand()->fallthrough();

    // options
    rangeApp->add_option("--SessionDataVersion", m_cliData->SessionData.sessionDataVersion)->capture_default_str();
    rangeApp->add_option("--SessionId", m_cliData->SessionData.sessionId)->capture_default_str();
    rangeApp->add_option("--SubSessionId", m_cliData->SessionData.subSessionId)->capture_default_str();

    // sub-commands
    m_rangeStartApp = AddSubcommandUwbRangeStart(rangeApp);
    m_rangeStopApp = AddSubcommandUwbRangeStop(rangeApp);

    return rangeApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRangeStart(CLI::App* parent)
{
    auto& uwbConfig = m_cliData->uwbConfiguration;
    auto rangeStartApp = parent->add_subcommand("start", "start ranging. Please refer to Table 53 of the FiRa CSML spec for more info on the options")->fallthrough();

    // TODO is there a way to put all the enums into a list of [optionName, optionDestination, optionMap] so we don't have to create the initializer list each time
    // TODO get rid of these strings, instead use a macro to extract the enum name
    rangeStartApp->add_option("--DeviceRole", uwbConfig.deviceRole)->transform(CLI::CheckedTransformer(detail::DeviceRoleMap))->capture_default_str();
    rangeStartApp->add_option("--RangingMethod", uwbConfig.rangingConfigurationMethod)->transform(CLI::CheckedTransformer(detail::RangingMethodMap))->capture_default_str();
    rangeStartApp->add_option("--MeasurementReportMode", uwbConfig.rangingConfigurationReportMode)->transform(CLI::CheckedTransformer(detail::MeasurementReportModeMap))->capture_default_str();
    rangeStartApp->add_option("--StsConfiguration", uwbConfig.stsConfiguration)->transform(CLI::CheckedTransformer(detail::StsConfigurationMap))->capture_default_str();
    rangeStartApp->add_option("--MultiNodeMode", uwbConfig.multiNodeMode)->transform(CLI::CheckedTransformer(detail::MultiNodeModeMap))->capture_default_str();
    rangeStartApp->add_option("--RangingMode", uwbConfig.rangingTimeStruct)->transform(CLI::CheckedTransformer(detail::RangingModeMap))->capture_default_str();
    rangeStartApp->add_option("--SchedulingMode", uwbConfig.schedulingMode)->transform(CLI::CheckedTransformer(detail::SchedulingModeMap))->capture_default_str();
    rangeStartApp->add_option("--Channel", uwbConfig.channel)->transform(CLI::CheckedTransformer(detail::ChannelMap))->capture_default_str();
    rangeStartApp->add_option("--StsPacketConfiguration", uwbConfig.rframeConfig)->transform(CLI::CheckedTransformer(detail::StsPacketConfigurationMap))->capture_default_str();
    rangeStartApp->add_option("--ConvolutionalCodeConstraintLength", uwbConfig.convolutionalCodeConstraintLength)->transform(CLI::CheckedTransformer(detail::ConvolutionalCodeConstraintLengthMap))->capture_default_str();
    rangeStartApp->add_option("--PrfMode", uwbConfig.prfMode)->transform(CLI::CheckedTransformer(detail::PrfModeMap))->capture_default_str();
    rangeStartApp->add_option("--UwbMacAddressFcsType", uwbConfig.macAddressFcsType)->transform(CLI::CheckedTransformer(detail::UwbMacAddressFcsTypeMap))->capture_default_str();

    // booleans
    rangeStartApp->add_flag("--controller,!--controlee", m_cliData->HostIsController, "default is controlee")->capture_default_str();
    rangeStartApp->add_flag("--HoppingMode", uwbConfig.hoppingMode)->capture_default_str();
    rangeStartApp->add_flag("--BlockStriding", uwbConfig.blockStriding)->capture_default_str();

    // TODO check for int sizes when parsing input
    rangeStartApp->add_option("--UwbInitiationTime", uwbConfig.uwbInitiationTime, "uint32_t")->capture_default_str();
    rangeStartApp->add_option("--Sp0PhySetNumber", uwbConfig.sp0PhySetNumber, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--Sp1PhySetNumber", uwbConfig.sp1PhySetNumber, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--Sp3PhySetNumber", uwbConfig.sp3PhySetNumber, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--PreableCodeIndex", uwbConfig.preableCodeIndex, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--SlotsPerRangingRound", uwbConfig.slotsPerRangingRound, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--MaxContentionPhaseLength", uwbConfig.maxContentionPhaseLength, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--SlotDuration", uwbConfig.slotDuration, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--RangingInterval", uwbConfig.rangingInterval, "uint16_t")->capture_default_str();
    rangeStartApp->add_option("--KeyRotationRate", uwbConfig.keyRotationRate, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--MaxRangingRoundRetry", uwbConfig.maxRangingRoundRetry, "uint16_t")->capture_default_str();
    rangeStartApp->add_option("--StaticRangingInfoVendorId", m_cliData->StaticRanging.VendorId, "uint16_t. If --SecureRangingInfo* options are used, this option will be overridden")->capture_default_str();

    // arrays
    rangeStartApp->add_option("--StaticRangingInfoInitializationVector", m_cliData->StaticRanging.InitializationVector, "array of uint8_t. If --SecureRangingInfo* options are used, this option will be overridden")->delimiter(':');

    // strings
    rangeStartApp->add_option("--FiraPhyVersion", uwbConfig.firaPhyVersionString)->capture_default_str();
    rangeStartApp->add_option("--FiraMacVersion", uwbConfig.firaMacVersionString)->capture_default_str();
    rangeStartApp->add_option("--ResultReportConfiguration", uwbConfig.resultReportConfigurationString)->capture_default_str();

    rangeStartApp->parse_complete_callback([this, &uwbConfig] {
        std::cout << "Selected parameters:" << std::endl;

        for (const auto& [optionName, optionSelected] :
            std::initializer_list<std::tuple<std::string_view, std::string_view>>{
                detail::GetEnumTypeAndValue(uwbConfig.deviceRole),
                detail::GetEnumTypeAndValue(uwbConfig.rangingConfigurationMethod),
                detail::GetEnumTypeAndValue(uwbConfig.rangingConfigurationReportMode),
                detail::GetEnumTypeAndValue(uwbConfig.stsConfiguration),
                detail::GetEnumTypeAndValue(uwbConfig.multiNodeMode),
                detail::GetEnumTypeAndValue(uwbConfig.rangingTimeStruct),
                detail::GetEnumTypeAndValue(uwbConfig.channel),
                detail::GetEnumTypeAndValue(uwbConfig.rframeConfig),
                detail::GetEnumTypeAndValue(uwbConfig.convolutionalCodeConstraintLength),
                detail::GetEnumTypeAndValue(uwbConfig.prfMode),
                detail::GetEnumTypeAndValue(uwbConfig.macAddressFcsType) }) {
            std::cout << optionName << "::" << optionSelected << std::endl;
        }
        if (!uwbConfig.firaMacVersionString.empty()) {
            auto result = uwb::protocol::fira::StringToVersion(uwbConfig.firaMacVersionString);
            if (not result) {
                std::cout << "could not parse MacVersionString" << std::endl;
            } else {
                uwbConfig.firaMacVersion = result.value();
            }
        }
        if (!uwbConfig.firaPhyVersionString.empty()) {
            auto result = uwb::protocol::fira::StringToVersion(uwbConfig.firaPhyVersionString);
            if (not result) {
                std::cout << "could not parse PhyVersionString" << std::endl;
            } else {
                uwbConfig.firaPhyVersion = result.value();
            }
        }

        if (!uwbConfig.resultReportConfigurationString.empty()) {
            auto result = uwb::protocol::fira::StringToResultReportConfiguration(uwbConfig.resultReportConfigurationString, detail::ResultReportConfigurationMap);
            if (not result) {
                std::cout << "could not parse ResultReportConfiguration" << std::endl;
            } else {
                uwbConfig.resultReportConfigurations = result.value();
            }
        }

        m_cliData->SessionData.staticRangingInfo = m_cliData->StaticRanging;
        std::cout << "StaticRangingInfo: {" << m_cliData->SessionData.staticRangingInfo << "}" << std::endl;
        std::cout << "ResultReportConfigurations: " << uwb::protocol::fira::ResultReportConfigurationToString(uwbConfig.resultReportConfigurations) << std::endl;
    });

    rangeStartApp->final_callback([this] {
        auto uwbDevice = GetUwbDevice();
        if (!uwbDevice) {
            return;
        }

        m_cliHandler->HandleStartRanging(uwbDevice, m_cliData->SessionData);
    });

    return rangeStartApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRangeStop(CLI::App* parent)
{
    // top-level command
    auto rangeStopApp = parent->add_subcommand("stop", "stop ranging")->fallthrough();

    rangeStopApp->parse_complete_callback([this] {
        std::cout << "stop ranging" << std::endl;
        m_cliHandler->HandleStopRanging();
    });

    return rangeStopApp;
}

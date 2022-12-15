
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <magic_enum.hpp>
#include <nearobject/cli/NearObjectCli.hxx>

using namespace nearobject::cli;

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
GetEnumTypeAndValue(const EnumT& value)
{
    return std::make_tuple(magic_enum::enum_type_name<EnumT>(), magic_enum::enum_name(value));
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
NearObjectCli::AddSubcommandUwb(CLI::App *parent)
{
    // top-level command
    auto uwbApp = parent->add_subcommand("uwb", "commands related to uwb")->require_subcommand()->fallthrough();

    // sub-commands
    m_rangeApp = AddSubcommandUwbRange(uwbApp);

    return uwbApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRange(CLI::App *parent)
{
    // top-level command
    auto rangeApp = parent->add_subcommand("range", "commands related to ranging")->require_subcommand()->fallthrough();

    // options
    rangeApp->add_option("--SessionDataVersion", m_cliData->SessionData.SessionDataVersion)->capture_default_str();
    rangeApp->add_option("--SessionId", m_cliData->SessionData.SessionId)->capture_default_str();
    rangeApp->add_option("--SubSessionId", m_cliData->SessionData.SubSessionId)->capture_default_str();

    // sub-commands
    m_rangeStartApp = AddSubcommandUwbRangeStart(rangeApp);
    m_rangeStopApp = AddSubcommandUwbRangeStop(rangeApp);

    return rangeApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRangeStart(CLI::App *parent)
{
    auto rangeStartApp = parent->add_subcommand("start", "start ranging. Please refer to Table 53 of the FiRa CSML spec for more info on the options")->fallthrough();

    // TODO is there a way to put all the enums into a list of [optionName, optionDestination, optionMap] so we don't have to create the initializer list each time
    // TODO get rid of these strings, instead use a macro to extract the enum name
    rangeStartApp->add_option("--DeviceRole", m_cliData->SessionData.UwbConfiguration.DeviceRole)->transform(CLI::CheckedTransformer(detail::DeviceRoleMap))->capture_default_str();
    rangeStartApp->add_option("--RangingMethod", m_cliData->SessionData.UwbConfiguration.RangingConfiguration.Method)->transform(CLI::CheckedTransformer(detail::RangingMethodMap))->capture_default_str();
    rangeStartApp->add_option("--MeasurementReportMode", m_cliData->SessionData.UwbConfiguration.RangingConfiguration.ReportMode)->transform(CLI::CheckedTransformer(detail::MeasurementReportModeMap))->capture_default_str();
    rangeStartApp->add_option("--StsConfiguration", m_cliData->SessionData.UwbConfiguration.StsConfiguration)->transform(CLI::CheckedTransformer(detail::StsConfigurationMap))->capture_default_str();
    rangeStartApp->add_option("--MultiNodeMode", m_cliData->SessionData.UwbConfiguration.MultiNodeMode)->transform(CLI::CheckedTransformer(detail::MultiNodeModeMap))->capture_default_str();
    rangeStartApp->add_option("--RangingMode", m_cliData->SessionData.UwbConfiguration.RangingTimeStruct)->transform(CLI::CheckedTransformer(detail::RangingModeMap))->capture_default_str();
    rangeStartApp->add_option("--SchedulingMode", m_cliData->SessionData.UwbConfiguration.SchedulingMode)->transform(CLI::CheckedTransformer(detail::SchedulingModeMap))->capture_default_str();
    rangeStartApp->add_option("--Channel", m_cliData->SessionData.UwbConfiguration.Channel)->transform(CLI::CheckedTransformer(detail::ChannelMap))->capture_default_str();
    rangeStartApp->add_option("--StsPacketConfiguration", m_cliData->SessionData.UwbConfiguration.RFrameConfig)->transform(CLI::CheckedTransformer(detail::StsPacketConfigurationMap))->capture_default_str();
    rangeStartApp->add_option("--ConvolutionalCodeConstraintLength", m_cliData->SessionData.UwbConfiguration.ConvolutionalCodeConstraintLength)->transform(CLI::CheckedTransformer(detail::ConvolutionalCodeConstraintLengthMap))->capture_default_str();
    rangeStartApp->add_option("--PrfMode", m_cliData->SessionData.UwbConfiguration.PrfMode)->transform(CLI::CheckedTransformer(detail::PrfModeMap))->capture_default_str();
    rangeStartApp->add_option("--UwbMacAddressFcsType", m_cliData->SessionData.UwbConfiguration.MacAddressFcsType)->transform(CLI::CheckedTransformer(detail::UwbMacAddressFcsTypeMap))->capture_default_str();

    // booleans
    rangeStartApp->add_flag("--controller,!--controlee", m_cliData->HostIsController, "default is controlee")->capture_default_str();
    rangeStartApp->add_flag("--HoppingMode", m_cliData->SessionData.UwbConfiguration.HoppingMode)->capture_default_str();
    rangeStartApp->add_flag("--BlockStriding", m_cliData->SessionData.UwbConfiguration.BlockStriding)->capture_default_str();

    // TODO check for int sizes when parsing input
    rangeStartApp->add_option("--UwbInitiationTime", m_cliData->SessionData.UwbConfiguration.UwbInitiationTime, "uint32_t")->capture_default_str();
    rangeStartApp->add_option("--Sp0PhySetNumber", m_cliData->SessionData.UwbConfiguration.Sp0PhySetNumber, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--Sp1PhySetNumber", m_cliData->SessionData.UwbConfiguration.Sp1PhySetNumber, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--Sp3PhySetNumber", m_cliData->SessionData.UwbConfiguration.Sp3PhySetNumber, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--PreableCodeIndex", m_cliData->SessionData.UwbConfiguration.PreableCodeIndex, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--SlotsPerRangingRound", m_cliData->SessionData.UwbConfiguration.SlotsPerRangingRound, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--MaxContentionPhaseLength", m_cliData->SessionData.UwbConfiguration.MaxContentionPhaseLength, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--SlotDuration", m_cliData->SessionData.UwbConfiguration.SlotDuration, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--RangingInterval", m_cliData->SessionData.UwbConfiguration.RangingInterval, "uint16_t")->capture_default_str();
    rangeStartApp->add_option("--KeyRotationRate", m_cliData->SessionData.UwbConfiguration.KeyRotationRate, "uint8_t")->capture_default_str();
    rangeStartApp->add_option("--MaxRangingRoundRetry", m_cliData->SessionData.UwbConfiguration.MaxRangingRoundRetry, "uint16_t")->capture_default_str();

    // strings
    rangeStartApp->add_option("--FiraPhyVersion", m_cliData->PhyVersionString)->capture_default_str();
    rangeStartApp->add_option("--FiraMacVersion", m_cliData->MacVersionString)->capture_default_str();
    rangeStartApp->add_option("--ResultReportConfiguration", m_cliData->ResultReportConfigurationString)->capture_default_str();

    rangeStartApp->parse_complete_callback([&] {
        std::cout << "Selected parameters:" << std::endl;

        for (const auto& [optionName, optionSelected] :
            std::initializer_list<std::tuple<std::string_view, std::string_view>>{
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.DeviceRole),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.RangingConfiguration.Method),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.RangingConfiguration.ReportMode),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.StsConfiguration),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.MultiNodeMode),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.RangingTimeStruct),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.Channel),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.RFrameConfig),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.ConvolutionalCodeConstraintLength),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.PrfMode),
                detail::GetEnumTypeAndValue(m_cliData->SessionData.UwbConfiguration.MacAddressFcsType) }) {
            std::cout << optionName << "::" << optionSelected << std::endl;
        }
        if (!m_cliData->MacVersionString.empty()) {
            auto result = uwb::protocol::fira::StringToVersion(m_cliData->MacVersionString);
            if (not result) {
                std::cout << "could not parse MacVersionString" << std::endl;
            } else {
                m_cliData->SessionData.UwbConfiguration.FiraMacVersion = result.value();
            }
        }
        if (!m_cliData->PhyVersionString.empty()) {
            auto result = uwb::protocol::fira::StringToVersion(m_cliData->PhyVersionString);
            if (not result) {
                std::cout << "could not parse PhyVersionString" << std::endl;
            } else {
                m_cliData->SessionData.UwbConfiguration.FiraPhyVersion = result.value();
            }
        }

        if (!m_cliData->ResultReportConfigurationString.empty()) {
            auto result = uwb::protocol::fira::StringToResultReportConfiguration(m_cliData->ResultReportConfigurationString, detail::ResultReportConfigurationMap);
            if (not result) {
                std::cout << "could not parse ResultReportConfiguration" << std::endl;
            } else {
                m_cliData->SessionData.UwbConfiguration.ResultReportConfigurations = result.value();
            }
        }

        std::cout << "FiRa MAC Version: " << std::setfill('0') << std::showbase << std::setw(8) << std::left << std::hex << m_cliData->SessionData.UwbConfiguration.FiraMacVersion << std::endl;
        std::cout << "FiRa PHY Version: " << std::setfill('0') << std::showbase << std::setw(8) << std::left << std::hex << m_cliData->SessionData.UwbConfiguration.FiraPhyVersion << std::endl;
        std::cout << "ResultReportConfigurations: " << uwb::protocol::fira::ResultReportConfigurationToString(m_cliData->SessionData.UwbConfiguration.ResultReportConfigurations) << std::endl;
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
NearObjectCli::AddSubcommandUwbRangeStop(CLI::App *parent)
{
    // top-level command
    auto rangeStopApp = parent->add_subcommand("stop", "stop ranging")->fallthrough();

    rangeStopApp->parse_complete_callback([this] {
        std::cout << "stop ranging" << std::endl;
        m_cliHandler->HandleStopRanging();
    });

    return rangeStopApp;
}


#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>
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
 * @brief Helper function to produce a mapping of enumeration strings to their
 * corresponding field names.
 *
 * This is used for CLI11 CheckedTransformer which validates string inputs and
 * maps them to their corresponding enumeration value.
 *
 * @tparam EnumType The type of the enumeration.
 * @return std::map<std::string, EnumType>
 */
template <typename EnumType>
// clang-format off
requires std::is_enum_v<EnumType>
const std::unordered_map<std::string, EnumType>
CreateEnumerationStringMap() noexcept
// clang-format on
{
    const auto& reverseMap = magic_enum::enum_entries<EnumType>();
    std::unordered_map<std::string, EnumType> map;

    for (const auto& [enumValue, enumName] : reverseMap) {
        map.emplace(enumName, enumValue);
    }

    return map;
}

/**
 * @brief Adds a cli option based on an enumeration.
 *
 * This uses the name of the enumeration as the option name and configures to
 * ignore case. For example, if the enum type MyEnum is used, then an option is
 * added with name --MyEnum and it can be specified with any case.
 *
 * @tparam EnumType The enumeration type to add as an option.
 * @param app The target CLI11 application to add the option to.
 * @param assignTo The destination variable to store the parsed option in.
 * @return The added option, which additional configuration can be applied to.
 */
template <typename EnumType>
// clang-format off
requires std::is_enum_v<EnumType>
CLI::Option*
AddEnumOption(CLI::App* app, std::optional<EnumType>& assignTo)
// clang-format on
{
    std::string optionName{ std::string("--").append(magic_enum::enum_type_name<EnumType>()) };

    return app->add_option(std::move(optionName), assignTo)
        ->transform(CLI::CheckedTransformer(CreateEnumerationStringMap<EnumType>(), CLI::ignore_case))
        ->capture_default_str();
}

} // namespace detail

CLI::App*
NearObjectCli::AddSubcommandUwb(CLI::App* parent)
{
    // top-level command
    auto uwbApp = parent->add_subcommand("uwb", "commands related to uwb")->require_subcommand()->fallthrough();

    // sub-commands
    m_monitorApp = AddSubcommandUwbMonitor(uwbApp);
    m_rangeApp = AddSubcommandUwbRange(uwbApp);

    return uwbApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbMonitor(CLI::App* parent)
{
    auto monitorApp = parent->add_subcommand("monitor", "commands relating to monitor mode")->fallthrough();

    monitorApp->final_callback([this] {
        m_cliHandler->HandleMonitorMode();
    });

    return monitorApp;
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

    // enumerations
    detail::AddEnumOption(rangeStartApp, uwbConfig.deviceRole);
    detail::AddEnumOption(rangeStartApp, uwbConfig.rangingDirection);
    detail::AddEnumOption(rangeStartApp, uwbConfig.rangingMeasurementReportMode);
    detail::AddEnumOption(rangeStartApp, uwbConfig.stsConfiguration);
    detail::AddEnumOption(rangeStartApp, uwbConfig.multiNodeMode);
    detail::AddEnumOption(rangeStartApp, uwbConfig.rangingTimeStruct);
    detail::AddEnumOption(rangeStartApp, uwbConfig.schedulingMode);
    detail::AddEnumOption(rangeStartApp, uwbConfig.channel);
    detail::AddEnumOption(rangeStartApp, uwbConfig.rframeConfig);
    detail::AddEnumOption(rangeStartApp, uwbConfig.convolutionalCodeConstraintLength);
    detail::AddEnumOption(rangeStartApp, uwbConfig.prfMode);
    detail::AddEnumOption(rangeStartApp, uwbConfig.macAddressFcsType);

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

    rangeStartApp->parse_complete_callback([this] {
        m_cliData->SessionData.uwbConfiguration = m_cliData->uwbConfiguration;
        m_cliData->SessionData.staticRangingInfo = m_cliData->StaticRanging;

        std::cout << "Selected parameters:" << std::endl;

        for (const auto& [parameterTag, parameterValue] : m_cliData->SessionData.uwbConfiguration.GetValueMap()) {
            std::visit([](auto&& arg) {
                using ParameterValueT = std::decay_t<decltype(arg)>;
                if constexpr (std::is_enum_v<ParameterValueT>) {
                    std::cout << magic_enum::enum_type_name<ParameterValueT>() << "::" << magic_enum::enum_name(arg) << std::endl;
                } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<uwb::protocol::fira::ResultReportConfiguration>>) {
                    std::cout << "ResultReportConfigurations: " << uwb::protocol::fira::ResultReportConfigurationToString(arg) << std::endl;
                }
            },
                parameterValue);
        }

        std::cout << "StaticRangingInfo: { " << m_cliData->SessionData.staticRangingInfo << " }" << std::endl;
    });

    rangeStartApp->final_callback([this] {
        auto uwbDevice = GetUwbDevice();
        if (!uwbDevice) {
            std::cerr << "no device found" << std::endl;
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

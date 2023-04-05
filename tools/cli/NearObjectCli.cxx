
#include <bitset>
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
using namespace ::uwb::protocol::fira;

NearObjectCli::NearObjectCli(std::shared_ptr<NearObjectCliData> cliData, std::shared_ptr<NearObjectCliHandler> cliHandler) :
    m_cliData(cliData),
    m_cliHandler(std::move(cliHandler)),
    m_cliApp(CreateParser())
{
    m_cliHandler->SetParent(this);

    if (!m_cliApp) {
        throw std::runtime_error("failed to create command line parser");
    }
}

std::shared_ptr<NearObjectCliData>
NearObjectCli::GetData() const noexcept
{
    return m_cliData;
}

std::shared_ptr<NearObjectCliControlFlowContext>
NearObjectCli::GetControlFlowContext() const noexcept
{
    return m_cliControlFlowContext;
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

void
NearObjectCli::RegisterCliAppWithOperation(CLI::App* app)
{
    m_cliAppOperations.insert(app);
}

void
NearObjectCli::SignalCliAppOperationCompleted(CLI::App* app)
{
    std::size_t numOperationsCompleted = m_cliAppOperations.erase(app);
    if (numOperationsCompleted > 0) {
        m_cliControlFlowContext->OperationSignalComplete(numOperationsCompleted);
    }
}

void
NearObjectCli::WaitForExecutionComplete()
{
    m_cliControlFlowContext->OperationsWaitForComplete();
}

void
NearObjectCli::CancelExecution()
{
    bool stopRequested = m_cliControlFlowContext->RequestStopExecution();
    if (!stopRequested) {
        // TODO: log?
    }
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
NearObjectCli::GetServiceApp() noexcept
{
    return *m_serviceApp;
}

CLI::App&
NearObjectCli::GetUwbRawApp() noexcept
{
    return *m_uwbRawApp;
}

CLI::App&
NearObjectCli::GetUwbRangeApp() noexcept
{
    return *m_uwbRangeApp;
}

CLI::App&
NearObjectCli::GetServiceRangeApp() noexcept
{
    return *m_serviceRangeApp;
}

CLI::App&
NearObjectCli::GetUwbRangeStartApp() noexcept
{
    return *m_uwbRangeStartApp;
}

CLI::App&
NearObjectCli::GetUwbRangeStopApp() noexcept
{
    return *m_uwbRangeStopApp;
}

CLI::App&
NearObjectCli::GetServiceRangeStartApp() noexcept
{
    return *m_serviceRangeStartApp;
}

CLI::App&
NearObjectCli::GetServiceRangeStopApp() noexcept
{
    return *m_serviceRangeStopApp;
}

std::shared_ptr<uwb::UwbDevice>
NearObjectCli::GetUwbDevice() noexcept
{
    auto uwbDevice = m_cliHandler->ResolveUwbDevice(*m_cliData);
    return std::move(uwbDevice);
}

std::unique_ptr<CLI::App>
NearObjectCli::CreateParser() noexcept
{
    // top-level command
    auto app = std::make_unique<CLI::App>("A command line tool to assist with all things nearobject", "nocli");
    app->require_subcommand();
    app->parse_complete_callback([this] {
        m_cliControlFlowContext = std::make_shared<NearObjectCliControlFlowContext>(std::size(m_cliAppOperations));
    });

    // sub-commands
    m_uwbApp = AddSubcommandUwb(app.get());
    m_serviceApp = AddSubcommandService(app.get());

    return app;
}

namespace detail
{
/**
 * @brief Helper function to produce a mapping of enumeration field names to their
 * corresponding values.
 *
 * @tparam EnumType The type of the enumeration.
 * @return std::unordered_map<std::string, EnumType>
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
 * This uses the name of the enumeration as the option name and gives
 * the usage as the description.
 *
 * @tparam EnumType The enumeration type to add as an option.
 * @param app The target CLI11 application to add the option to.
 * @param assignTo The destination variable to store the parsed option in.
 * @param isMandatory Flag that determines whether or not the option is mandatory.
 * @return The added option, which additional configuration can be applied to.
 */
template <typename EnumType>
// clang-format off
requires std::is_enum_v<EnumType>
CLI::Option*
AddEnumOption(CLI::App* app, std::optional<EnumType>& assignTo, bool isMandatory = false)
// clang-format on
{
    std::string optionName{ std::string("--").append(magic_enum::enum_type_name<EnumType>()) };

    const auto map = CreateEnumerationStringMap<EnumType>();
    std::ostringstream enumUsage;

    auto it = std::cbegin(map);
    const auto& [name, value] = *it;
    enumUsage << "value in { " << static_cast<int>(value) << "(" << name << ")";
    for (it = std::next(std::cbegin(map)); it != std::cend(map); std::advance(it, 1)) {
        const auto& [name, value] = *it;
        enumUsage << ", " << static_cast<int>(value) << "(" << name << ")";
    }
    enumUsage << " }";

    if (isMandatory) {
        return app->add_option(std::move(optionName), assignTo, enumUsage.str())->capture_default_str()->required();
    } else {
        return app->add_option(std::move(optionName), assignTo, enumUsage.str())->capture_default_str();
    }
}

/**
 * @brief Validates non-enum application configuration parameter inputs from nocli.
 *
 * @param cliData The nocli input data containing the application configuration parameters
 */
void
ValidateNonEnumParameterValues(NearObjectCliData& cliData)
{
    // Note: No restrictions other than type bounds checking given in FiRa UCI Generic Technical Specification v1.1.0 for the following parameters:
    // SlotDuration, SlotsPerRangingRound, VendorId, MaxRangingRoundRetry, BlockStrideLength, MaxNumberOfMeasurements, RangingInterval, StsIndex,
    // HoppingMode, StaticStsIv

    auto& parametersData = cliData.applicationConfigurationParametersData;

    // NumberOfControlees (mandatory)
    if (parametersData.multiNodeMode == MultiNodeMode::Unicast) {
        if (parametersData.numberOfControlees != MinimumNumberOfControlees) {
            std::cerr << "Invalid NumberOfControlees. Only " << +MinimumNumberOfControlees << " controlee expected in Unicast mode " << std::endl;
        }
    } else {
        if (parametersData.numberOfControlees < MinimumNumberOfControlees) {
            std::cerr << "Invalid NumberOfControlees. At least " << +MinimumNumberOfControlees << " controlees expected" << std::endl;
        }
    }

    // DeviceMacAddress (mandatory) and DestinationMacAddresses (mandatory)
    const auto macAddressType = parametersData.macAddressMode == uwb::UwbMacAddressType::Extended ? uwb::UwbMacAddressType::Extended : uwb::UwbMacAddressType::Short;
    parametersData.deviceMacAddress = uwb::UwbMacAddress::FromString(cliData.deviceMacAddressString, macAddressType);
    parametersData.destinationMacAddresses.emplace();
    // TODO: Insert the correct Controlee mac address rather than simply inserting the entire destinationMacAddressString
    for (auto i = 0; i < parametersData.numberOfControlees; i++) {
        parametersData.destinationMacAddresses.value().insert(uwb::UwbMacAddress::FromString(cliData.destinationMacAddressString, macAddressType).value());
    }

    // RangeDataNotificationProximityNear
    if (parametersData.rangeDataNotificationProximityNear.has_value()) {
        if (parametersData.rangeDataNotificationConfig.has_value() && parametersData.rangeDataNotificationConfig.value() == RangeDataNotificationConfiguration::EnableInProximityRange) {
            if ((!parametersData.rangeDataNotificationProximityFar.has_value() && parametersData.rangeDataNotificationProximityNear.value() > DefaultRangeDataNotificationProximityFar) ||
                (parametersData.rangeDataNotificationProximityFar.has_value() && parametersData.rangeDataNotificationProximityNear.value() > parametersData.rangeDataNotificationProximityFar.value())) {
                std::cerr << "Invalid RangeDataNotificationProximityNear. Should be less than or equal to RangeDataNotificationProximityFar (default: " << +DefaultRangeDataNotificationProximityFar << ")." << std::endl;
            }
        }
    }

    // RangeDataNotificationProximityFar
    if (parametersData.rangeDataNotificationProximityFar.has_value()) {
        if (parametersData.rangeDataNotificationConfig.has_value() && parametersData.rangeDataNotificationConfig.value() == RangeDataNotificationConfiguration::EnableInProximityRange) {
            if (parametersData.rangeDataNotificationProximityNear.has_value() && parametersData.rangeDataNotificationProximityFar.value() < parametersData.rangeDataNotificationProximityNear.value()) {
                std::cerr << "Invalid RangeDataNotificationProximityFar. Should be greater than or equal to RangeDataNotificationProximityNear (default: " << +DefaultRangeDataNotificationProximityNear << ")." << std::endl;
            }
        }
    }

    // PreambleCodeIndex
    if (parametersData.preambleCodeIndex.has_value()) {
        if (!parametersData.prfMode.has_value() || (parametersData.prfMode.has_value() && parametersData.prfMode.value() == PrfModeDetailed::Bprf62MHz)) { // Either BPRF is set or PRF_MODE is left at default (BPRF)
            if (parametersData.preambleCodeIndex.value() < MinimumPreambleCodeIndexBprf || parametersData.preambleCodeIndex.value() > MaximumPreambleCodeIndexBprf) {
                std::cerr << "Invalid PreambleCodeIndex. Expected value range of " << +MinimumPreambleCodeIndexBprf << "-" << +MaximumPreambleCodeIndexBprf << " in BPRF mode." << std::endl;
            }
        } else { // HPRF mode
            if (parametersData.preambleCodeIndex.value() < MinimumPreambleCodeIndexHprf || parametersData.preambleCodeIndex.value() > MaximumPreambleCodeIndexHprf) {
                std::cerr << "Invalid PreambleCodeIndex. Expected value range of " << +MinimumPreambleCodeIndexHprf << "-" << +MaximumPreambleCodeIndexHprf << " in BPRF mode." << std::endl;
            }
        }
    }

    // SfdId
    if (parametersData.sfdId.has_value()) {
        if (!parametersData.prfMode.has_value() || (parametersData.prfMode.has_value() && parametersData.prfMode.value() == PrfModeDetailed::Bprf62MHz)) { // Either BPRF is set or PRF_MODE is left at default (BPRF)
            if (std::ranges::find(SfdIdValuesBprf, parametersData.sfdId.value()) == std::ranges::end(SfdIdValuesBprf)) {
                std::string errorMessage = "Invalid SfdId. Expected values of { ";
                for (const auto& val : SfdIdValuesBprf) {
                    errorMessage.append(std::to_string(val) + " ");
                }
                errorMessage.append("} in BPRF mode.");
                std::cerr << errorMessage << std::endl;
            }
        } else { // HPRF mode
            if (std::ranges::find(SfdIdValuesHprf, parametersData.sfdId.value()) == std::ranges::end(SfdIdValuesHprf)) {
                std::string errorMessage = "Invalid SfdId. Expected values of { ";
                for (const auto& val : SfdIdValuesHprf) {
                    errorMessage.append(std::to_string(val) + " ");
                }
                errorMessage.append("} in HPRF mode.");
                std::cerr << errorMessage << std::endl;
            }
        }
    }

    // ResponderSlotIndex
    if (parametersData.responderSlotIndex.has_value() && parametersData.responderSlotIndex.value() < MinimumResponderSlotIndex) { // TODO: > N, where N is number of Responders
        std::cerr << "Invalid ResponderSlotIndex. Expected value range of " << +MinimumResponderSlotIndex << "- N Responders." << std::endl;
    }

    // KeyRotationRate
    if (parametersData.keyRotationRate.has_value() && parametersData.keyRotationRate.value() > MaximumKeyRotationRate) {
        std::cerr << "Invalid KeyRotationRate. Expected value range of " << +MinimumKeyRotationRate << "-" << +MaximumKeyRotationRate << std::endl;
    }

    // SessionPriority
    if (parametersData.sessionPriority.has_value() && (parametersData.sessionPriority.value() < MinimumSessionPriority || parametersData.sessionPriority.value() > MaximumSessionPriority)) {
        std::cerr << "Invalid SessionPriority. Expected value range of " << +MinimumSessionPriority << "-" << +MaximumSessionPriority << std::endl;
    }

    // NumberOfStsSegments
    if (parametersData.numberOfStsSegments.has_value()) {
        if (parametersData.numberOfStsSegments.value() > MaximumNumberOfStsSegmentsHprf) {
            std::cerr << "Invalid NumberOfStsSegments. Expected value range of " << +MinimumNumberOfStsSegments << "-" << +MaximumNumberOfStsSegmentsHprf << " STS segments." << std::endl;
        }
        if (!parametersData.prfMode.has_value() || (parametersData.prfMode.has_value() && parametersData.prfMode.value() == PrfModeDetailed::Bprf62MHz)) { // Either BPRF is set or PRF_MODE is left at default (BPRF)
            if (parametersData.numberOfStsSegments.value() > MaximumNumberOfStsSegmentsBprf) {
                std::cerr << "Invalid NumberOfStsSegments. Expected value range of " << +MinimumNumberOfStsSegments << "-" << +MaximumNumberOfStsSegmentsBprf << " in BPRF mode." << std::endl;
            }
        }
        if (parametersData.rFrameConfiguration.value() == StsPacketConfiguration::SP0 && parametersData.numberOfStsSegments != MinimumNumberOfStsSegments) {
            std::cerr << "Invalid NumberOfStsSegments. No STS segments expected with non-STS frames" << std::endl;
        }
    }

    // UwbInitiationTime
    if (parametersData.uwbInitiationTime.has_value() && parametersData.uwbInitiationTime.value() > MaximumUwbInitiationTime) {
        std::cerr << "Invalid UwbInitiationTime. Expected value range of " << +MinimumUwbInitiationTime << "-" << +MaximumUwbInitiationTime << std::endl;
    }

    // ResultReportConfig
    constexpr int resultReportConfigurationSize = magic_enum::enum_count<ResultReportConfiguration>();
    auto IsValidResultReportConfigurationString = [resultReportConfigurationSize](const std::string& resultReportConfigurationString) {
        if (resultReportConfigurationString.length() != resultReportConfigurationSize) {
            std::cerr << "Invalid ResultReportConfiguration length" << std::endl;
            return false;
        }
        return std::ranges::all_of(resultReportConfigurationString, [](const auto& c) {
            return c == '0' || c == '1';
        });
    };
    if (IsValidResultReportConfigurationString(cliData.resultReportConfigurationString)) {
        parametersData.resultReportConfig.emplace();

        const std::bitset<resultReportConfigurationSize> resultReportConfigurationBits(cliData.resultReportConfigurationString);
        for (auto i = 0; i < resultReportConfigurationSize; i++) {
            if (resultReportConfigurationBits.test(i)) {
                parametersData.resultReportConfig.value().insert(magic_enum::enum_value<ResultReportConfiguration>(i));
            }
        }
    }

    // InBandTerminationAttemptCount
    if (parametersData.inBandTerminationAttemptCount.has_value() && parametersData.inBandTerminationAttemptCount.value() > MaximumInBandTerminationAttemptCount) {
        std::cerr << "Invalid InBandTerminationAttemptCount. Expected value range of " << +MinimumInBandTerminationAttemptCount << "-" << +MaximumInBandTerminationAttemptCount << std::endl;
    }

    // SubSessionId
    if (parametersData.stsConfiguration.has_value() && parametersData.stsConfiguration.value() == StsConfiguration::DynamicWithResponderSubSessionKey) {
        if (!parametersData.subSessionId.has_value()) {
            std::cerr << "SubSessionId is required for Dynamic STS with Responder Sub-Session Key" << std::endl;
        }
    }

    // BprfPhrDataRate
    if (parametersData.prfMode.has_value() && parametersData.prfMode.value() != PrfModeDetailed::Bprf62MHz) {
        if (parametersData.bprfPhrDataRate.has_value()) {
            std::cerr << "Invalid BprfPhrDataRate. Value expected only in BPRF mode" << std::endl;
        }
    } else {
        if (parametersData.bprfPhrDataRate.has_value() && !magic_enum::enum_contains<BprfPhrDataRate>(parametersData.bprfPhrDataRate.value())) {
            std::cerr << "Invalid BprfPhrDataRate" << std::endl;
        }
    }
}

/**
 * @brief Validates the enum application configuration parameter inputs from nocli.
 *
 * @tparam EnumType The type of the enumeration.
 * @param applicationConfigurationParameter The application configuration parameter of type EnumType.
 */
template <typename EnumType>
// clang-format off
requires std::is_enum_v<EnumType>
void
ValidateEnumParameterValue(const EnumType& applicationConfigurationParameter)
// clang-format on
{
    auto parameterType = magic_enum::enum_type_name<EnumType>();
    if (!magic_enum::enum_contains<EnumType>(applicationConfigurationParameter)) {
        std::cerr << "Invalid " << parameterType << std::endl;
    }
}

/**
 * @brief Validates, displays, and returns application configuration parameters.
 *
 * @param cliData The nocli input data containing the application configuration parameters.
 * @return std::vector<uwb::protocol::fira::UwbApplicationConfigurationParameter>.
 */
std::vector<uwb::protocol::fira::UwbApplicationConfigurationParameter>
ProcessApplicationConfigurationParameters(NearObjectCliData& cliData)
{
    detail::ValidateNonEnumParameterValues(cliData);
    auto& applicationConfigurationParameterData = cliData.applicationConfigurationParametersData;

    std::vector<uwb::protocol::fira::UwbApplicationConfigurationParameter> applicationConfigurationParameters;

    std::cout << "Selected parameters:" << std::endl;
    const auto applicationConfigurationParameterValues = applicationConfigurationParameterData.GetValueMap();
    for (const auto& [applicationConfigurationParameter, applicationConfigurationParameterValue] : applicationConfigurationParameterValues) {
        std::visit([&applicationConfigurationParameter, &applicationConfigurationParameters](auto&& arg) {
            applicationConfigurationParameters.push_back({ applicationConfigurationParameter, arg });

            using ParameterValueT = std::decay_t<decltype(arg)>;
            std::ostringstream oss;
            oss << magic_enum::enum_name(applicationConfigurationParameter) << "::";
            if constexpr (std::is_enum_v<ParameterValueT>) {
                ValidateEnumParameterValue(arg);
                oss << magic_enum::enum_name(arg);
            } else if constexpr (std::is_same_v<ParameterValueT, bool>) {
                oss << std::boolalpha << arg;
            } else if constexpr (std::is_unsigned_v<ParameterValueT>) {
                oss << +arg;
            } else if constexpr (std::is_same_v<ParameterValueT, ::uwb::UwbMacAddress>) {
                oss << ToString(arg);
            } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<::uwb::UwbMacAddress>>) {
                for (const auto& address : arg) {
                    oss << ToString(arg);
                }
            } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<ResultReportConfiguration>>) {
                oss << ToString(arg);
            } else if constexpr (std::is_same_v<ParameterValueT, std::array<uint8_t, StaticStsInitializationVectorLength>>) {
                for (const auto& value : arg) {
                    oss << "0x" << std::setw(2) << std::internal << std::setfill('0') << std::hex << +value << " ";
                }
            }

            std::cout << oss.str() << std::endl;
        },
            applicationConfigurationParameterValue);
    }

    return applicationConfigurationParameters;
}

} // namespace detail

CLI::App*
NearObjectCli::AddSubcommandUwb(CLI::App* parent)
{
    // top-level command
    auto uwbApp = parent->add_subcommand("uwb", "commands related to UWB driver testing")->require_subcommand()->fallthrough();

    // sub-commands
    m_monitorApp = AddSubcommandUwbMonitor(uwbApp);
    m_uwbRawApp = AddSubcommandUwbRaw(uwbApp);
    m_uwbRangeApp = AddSubcommandUwbRange(uwbApp);

    return uwbApp;
}

CLI::App*
NearObjectCli::AddSubcommandService(CLI::App* parent)
{
    // top-level command
    auto serviceApp = parent->add_subcommand("service", "commands related to NearObject service testing")->require_subcommand()->fallthrough();

    // sub-commands
    m_serviceRangeApp = AddSubcommandServiceRange(serviceApp);

    return serviceApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbMonitor(CLI::App* parent)
{
    auto monitorApp = parent->add_subcommand("monitor", "commands relating to monitor mode")->fallthrough();

    monitorApp->parse_complete_callback([this, monitorApp] {
        RegisterCliAppWithOperation(monitorApp);
    });
    monitorApp->final_callback([this, monitorApp] {
        m_cliHandler->HandleMonitorMode();
        SignalCliAppOperationCompleted(monitorApp);
    });

    return monitorApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRaw(CLI::App* parent)
{
    // top-level command
    auto rawApp = parent->add_subcommand("raw", "individual commands for driver testing")->require_subcommand()->fallthrough();

    // sub-commands
    AddSubcommandUwbRawDeviceReset(rawApp);
    AddSubcommandUwbRawGetDeviceInfo(rawApp);
    AddSubcommandUwbRawSessionDeinitialize(rawApp);

    return rawApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRange(CLI::App* parent)
{
    // top-level command
    auto rangeApp = parent->add_subcommand("range", "commands related to ranging")->require_subcommand()->fallthrough();

    // options
    rangeApp->add_option("--SessionId", m_cliData->RangingParameters.SessionId)->capture_default_str()->required();

    // sub-commands
    m_uwbRangeStartApp = AddSubcommandUwbRangeStart(rangeApp);
    m_uwbRangeStopApp = AddSubcommandUwbRangeStop(rangeApp);

    return rangeApp;
}

CLI::App*
NearObjectCli::AddSubcommandServiceRange(CLI::App* parent)
{
    // top-level command
    auto rangeApp = parent->add_subcommand("range", "commands related to ranging")->require_subcommand()->fallthrough();

    // options
    rangeApp->add_option("--SessionDataVersion", m_cliData->SessionData.sessionDataVersion)->capture_default_str();
    rangeApp->add_option("--SessionId", m_cliData->SessionData.sessionId)->capture_default_str();
    rangeApp->add_option("--SubSessionId", m_cliData->SessionData.subSessionId)->capture_default_str();

    // sub-commands
    m_serviceRangeStartApp = AddSubcommandServiceRangeStart(rangeApp);
    m_serviceRangeStopApp = AddSubcommandServiceRangeStop(rangeApp);

    return rangeApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRawDeviceReset(CLI::App* parent)
{
    // top-level command
    auto rawDeviceResetApp = parent->add_subcommand("devicereset", "DeviceReset")->fallthrough();

    rawDeviceResetApp->parse_complete_callback([this] {
        std::cout << "device reset" << std::endl;
    });

    rawDeviceResetApp->final_callback([this] {
        auto uwbDevice = GetUwbDevice();
        if (!uwbDevice) {
            std::cerr << "no device found" << std::endl;
            return;
        }
        if (!uwbDevice->Initialize()) {
            std::cerr << "device not initialized" << std::endl;
        }

        m_cliHandler->HandleDeviceReset(uwbDevice);
    });

    return rawDeviceResetApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRawGetDeviceInfo(CLI::App* parent)
{
    // top-level command
    auto rawGetDeviceInfoApp = parent->add_subcommand("getdeviceinfo", "GetDeviceInfo")->fallthrough();

    rawGetDeviceInfoApp->parse_complete_callback([this] {
        std::cout << "get device info" << std::endl;
    });

    rawGetDeviceInfoApp->final_callback([this] {
        auto uwbDevice = GetUwbDevice();
        if (!uwbDevice) {
            std::cerr << "no device found" << std::endl;
            return;
        }
        if (!uwbDevice->Initialize()) {
            std::cerr << "device not initialized" << std::endl;
        }

        m_cliHandler->HandleGetDeviceInfo(uwbDevice);
    });

    return rawGetDeviceInfoApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRawSessionDeinitialize(CLI::App* parent)
{
    // top-level command
    auto rawSessionDeinitializeApp = parent->add_subcommand("sessiondeinit", "Deinitialize a pre-existing session")->fallthrough();
    rawSessionDeinitializeApp->add_option("Session Id, --SessionId", m_cliData->SessionId)->required();

    rawSessionDeinitializeApp->parse_complete_callback([this, rawSessionDeinitializeApp] {
        RegisterCliAppWithOperation(rawSessionDeinitializeApp);
        std::cout << "deinitialize session " << m_cliData->SessionId << std::endl;
    });

    rawSessionDeinitializeApp->final_callback([this, rawSessionDeinitializeApp] {
        auto uwbDevice = GetUwbDevice();
        if (!uwbDevice) {
            std::cerr << "no device found" << std::endl;
            return;
        }
        if (!uwbDevice->Initialize()) {
            std::cerr << "device not initialized" << std::endl;
        }

        m_cliHandler->HandleSessionDeinitialize(uwbDevice, m_cliData->SessionId);

        SignalCliAppOperationCompleted(rawSessionDeinitializeApp);
    });

    return rawSessionDeinitializeApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRangeStart(CLI::App* parent)
{
    auto& applicationConfigurationParametersData = m_cliData->applicationConfigurationParametersData;
    auto rangeStartApp = parent->add_subcommand("start", "start ranging. Please refer to Table 29 of the FiRa UCI spec for more info on the options")->fallthrough();

    // List mandatory params first
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.deviceRole, true);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.multiNodeMode, true);
    rangeStartApp->add_option("--NumberOfControlees", applicationConfigurationParametersData.numberOfControlees, "1 <= N <= 8")->capture_default_str()->required();

    std::string deviceMacAddressDescription = "2-byte short MAC address of own device: e.g. 12:34";
    std::string dstMacAddressDescription = "2-byte short MAC address(es) of other device(s). If device is Controller, list NumberOfControlees mac addresses. If device is Controlee, list Controller mac address";
    if (applicationConfigurationParametersData.macAddressMode == uwb::UwbMacAddressType::Extended) { // TODO: Enable macAddressMode to be set before checking this value. Otherwise, the default empty value will always be used
        deviceMacAddressDescription = "8-byte extended MAC address of own device: e.g. 12:34:56:78:87:65:43:21";
        dstMacAddressDescription = "8-byte extended MAC address(es) of other device(s). If device is Controller, list NumberOfControlees mac addresses. If device is Controlee, list Controller mac address";
    }
    rangeStartApp->add_option("--DeviceMacAddress", m_cliData->deviceMacAddressString, deviceMacAddressDescription)->capture_default_str()->required();
    rangeStartApp->add_option("--DestinationMacAddress", m_cliData->destinationMacAddressString, dstMacAddressDescription)->capture_default_str()->required();
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.deviceType, true);

    // List remaining params
    // uint8_t
    rangeStartApp->add_option("--BlockStrideLength", applicationConfigurationParametersData.blockStrideLength, "0 = No block striding; 1-255 = Number of ranging blocks to be skipped")->capture_default_str();
    rangeStartApp->add_option("--InBandTerminationAttemptCount", applicationConfigurationParametersData.inBandTerminationAttemptCount, "0 = Disable in-band termination attempt; 1-10 = In-band termination attempt count")->capture_default_str();
    rangeStartApp->add_option("--KeyRotationRate", applicationConfigurationParametersData.keyRotationRate, "Exponent n where 2^n is the key rotation rate. Value range: 0-15")->capture_default_str();
    rangeStartApp->add_option("--NumberOfStsSegments", applicationConfigurationParametersData.numberOfStsSegments, "Value range: 0-4. Note: 2-4 for HPRF Mode only")->capture_default_str();
    rangeStartApp->add_option("--PreambleCodeIndex", applicationConfigurationParametersData.preambleCodeIndex, "Value range: 9-12 for BPRF Mode; 25-32 for HPRF Mode")->capture_default_str();
    rangeStartApp->add_option("--ResponderSlotIndex", applicationConfigurationParametersData.responderSlotIndex, "Responder index in TWR. 1=Responder 1 ... N=Responder N. Not applicable to Initiator")->capture_default_str();
    rangeStartApp->add_option("--SessionPriority", applicationConfigurationParametersData.sessionPriority, "Value range: 1-100")->capture_default_str();
    rangeStartApp->add_option("--SfdId", applicationConfigurationParametersData.sfdId, "{0,2} for BPRF Mode; {1,2,3,4} for HPRF Mode")->capture_default_str();
    rangeStartApp->add_option("--SlotsPerRangingRound", applicationConfigurationParametersData.slotsPerRangingRound, "Number of slots per ranging round")->capture_default_str();

    // uint16_t
    rangeStartApp->add_option("--MaxNumberOfMeasurements", applicationConfigurationParametersData.maxNumberOfMeasurements, "0 = Unlimited; 1+ = Max number of ranging measurements in a session")->capture_default_str();
    rangeStartApp->add_option("--MaxRangingRoundRetry", applicationConfigurationParametersData.maxRangingRoundRetry, "Number of failed RR attempts before stopping the session. Value range: 0-65535")->capture_default_str();
    rangeStartApp->add_option("--RangeDataNotificationProximityFar", applicationConfigurationParametersData.rangeDataNotificationProximityFar, "Upper bound in cm for ranging proximity mode. N >= RangeDataNotificationProximityNear")->capture_default_str();
    rangeStartApp->add_option("--RangeDataNotificationProximityNear", applicationConfigurationParametersData.rangeDataNotificationProximityNear, "Lower bound in cm for ranging proximity mode. N <= RangeDataNotificationProximityFar")->capture_default_str();
    rangeStartApp->add_option("--SlotDuration", applicationConfigurationParametersData.slotDuration, "Duration of a ranging slot in the unit of RSTU")->capture_default_str();
    rangeStartApp->add_option("--VendorId", applicationConfigurationParametersData.vendorId, "Unique ID for vendor. Used for static STS")->capture_default_str();

    // uint32_t
    rangeStartApp->add_option("--RangingInterval", applicationConfigurationParametersData.rangingInterval, "Ranging interval in the unit of 1200 RSTU (1ms) between ranging rounds. Minimum should be duration of one ranging round")->capture_default_str();
    rangeStartApp->add_option("--StsIndex", applicationConfigurationParametersData.stsIndex, "Test Mode only. See FiRa Consortium PHY Conformance Test Specification")->capture_default_str();
    rangeStartApp->add_option("--SubSessionId", applicationConfigurationParametersData.subSessionId, "Sub-session ID for the controlee device. Required for Dynamic STS with responder specific sub-session key")->capture_default_str();
    rangeStartApp->add_option("--UwbInitiationTime", applicationConfigurationParametersData.uwbInitiationTime, "Value range: 0-10000")->capture_default_str();

    // bool
    rangeStartApp->add_flag("--HoppingMode", applicationConfigurationParametersData.hoppingMode, "Enables FiRa hopping");

    // enums
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.aoaResultRequest);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.bprfPhrDataRate);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.channelNumber);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.keyRotation);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.macAddressMode);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.macFcsType);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.preambleDuration);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.prfMode);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.rangeDataNotificationConfig);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.rangingRoundControl);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.rFrameConfiguration);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.scheduledMode);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.stsConfiguration);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.stsLength);
    detail::AddEnumOption(rangeStartApp, applicationConfigurationParametersData.txAdaptivePayloadPower);

    // other
    rangeStartApp->add_option("--ResultReportConfig", m_cliData->resultReportConfigurationString, "4-bit report config, e.g. 0101. b3=AOA FOM, b2=AOA Elevation, b1=AOA Azimuth, b0=TOF")->capture_default_str();
    rangeStartApp->add_option("--StaticStsInitializationVector", applicationConfigurationParametersData.staticStsIv, "6-octet array for vendor-defined static STS initialization vector, e.g. 11:22:33:44:55:66")->delimiter(':');

    rangeStartApp->parse_complete_callback([this, rangeStartApp] {
        m_cliData->RangingParameters.ApplicationConfigurationParameters = detail::ProcessApplicationConfigurationParameters(*m_cliData);
        RegisterCliAppWithOperation(rangeStartApp);
    });

    rangeStartApp->final_callback([this] {
        auto uwbDevice = GetUwbDevice();
        if (!uwbDevice) {
            std::cerr << "no device found" << std::endl;
            return;
        }
        if (!uwbDevice->Initialize()) {
            std::cerr << "device not initialized" << std::endl;
        }

        m_cliHandler->HandleDriverStartRanging(uwbDevice, m_cliData->RangingParameters);
    });

    return rangeStartApp;
}

CLI::App*
NearObjectCli::AddSubcommandUwbRangeStop(CLI::App* parent)
{
    // top-level command
    auto rangeStopApp = parent->add_subcommand("stop", "stop ranging")->fallthrough();

    rangeStopApp->parse_complete_callback([this, rangeStopApp] {
        std::cout << "stop ranging" << std::endl;
        RegisterCliAppWithOperation(rangeStopApp);
    });

    rangeStopApp->final_callback([this, rangeStopApp] {
        m_cliHandler->HandleStopRanging();
        SignalCliAppOperationCompleted(rangeStopApp);
    });

    return rangeStopApp;
}

CLI::App*
NearObjectCli::AddSubcommandServiceRangeStart(CLI::App* parent)
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
    rangeStartApp->add_option("--PreambleCodeIndex", uwbConfig.preambleCodeIndex, "uint8_t")->capture_default_str();
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

    rangeStartApp->parse_complete_callback([this, rangeStartApp] {
        m_cliData->SessionData.uwbConfiguration = m_cliData->uwbConfiguration;
        m_cliData->SessionData.staticRangingInfo = m_cliData->StaticRanging;

        std::cout << "Selected parameters:" << std::endl;

        for (const auto& [parameterTag, parameterValue] : m_cliData->SessionData.uwbConfiguration.GetValueMap()) {
            std::visit([](auto&& arg) {
                using ParameterValueT = std::decay_t<decltype(arg)>;
                if constexpr (std::is_enum_v<ParameterValueT>) {
                    std::cout << magic_enum::enum_type_name<ParameterValueT>() << "::" << magic_enum::enum_name(arg) << std::endl;
                } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<ResultReportConfiguration>>) {
                    std::cout << "ResultReportConfigurations: " << ToString(arg) << std::endl;
                }
            },
                parameterValue);
        }

        std::cout << "StaticRangingInfo: { " << m_cliData->SessionData.staticRangingInfo << " }" << std::endl;
        RegisterCliAppWithOperation(rangeStartApp);
    });

    rangeStartApp->final_callback([this] {
        auto uwbDevice = GetUwbDevice();
        if (!uwbDevice) {
            std::cerr << "no device found" << std::endl;
            return;
        }
        if (!uwbDevice->Initialize()) {
            std::cerr << "device not initialized" << std::endl;
        }

        m_cliHandler->HandleStartRanging(uwbDevice, m_cliData->SessionData);
    });

    return rangeStartApp;
}

CLI::App*
NearObjectCli::AddSubcommandServiceRangeStop(CLI::App* parent)
{
    // top-level command
    auto rangeStopApp = parent->add_subcommand("stop", "stop ranging")->fallthrough();

    rangeStopApp->parse_complete_callback([this, rangeStopApp] {
        std::cout << "stop ranging" << std::endl;
        RegisterCliAppWithOperation(rangeStopApp);
    });

    rangeStopApp->final_callback([this, rangeStopApp] {
        m_cliHandler->HandleStopRanging();
        SignalCliAppOperationCompleted(rangeStopApp);
    });

    return rangeStopApp;
}

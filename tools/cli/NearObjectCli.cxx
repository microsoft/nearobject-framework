
#include <stdexcept>

#include <magic_enum.hpp>
#include <nearobject/cli/NearObjectCli.hxx>

using namespace nearobject::cli;

NearObjectCli::NearObjectCli(std::shared_ptr<NearObjectCliData> cliData) :
    m_cliData(cliData)
{}

CLI::App&
NearObjectCli::GetParser()
{
    if (!m_cliApp) {
        m_cliApp = CreateParser();
        if (!m_cliApp) {
            throw std::runtime_error("failed to create command line parser");
        }
    }

    return *m_cliApp;
}

std::shared_ptr<NearObjectCliData>
NearObjectCli::GetData() const noexcept
{
    return m_cliData;
}

int
NearObjectCli::Parse(int argc, char* argv[])
{
    try {
        m_cliApp->parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return m_cliApp->exit(e);
    }

    return 0;
}

/**
 * @brief Helper function to populate the string-enum mapping
 * 
 * @tparam EnumType 
 * @return std::map<std::string, EnumType> 
 */
template <typename EnumType>
std::unordered_map<std::string, EnumType>
populate_map()
{
    auto reverseMap = magic_enum::enum_entries<EnumType>();
    std::vector<std::pair<std::string, EnumType>> destvector{ reverseMap.size() };
    std::transform(std::begin(reverseMap),
        std::end(reverseMap),
        std::begin(destvector),
        [](const auto& input) {
            return std::pair{ std::string{ input.second }, input.first };
        });
    return { std::cbegin(destvector), std::cend(destvector) };
}

std::unique_ptr<CLI::App>
NearObjectCli::CreateParser()
{
    // generate the maps
    {
        m_cliData->DeviceRoleMap = populate_map<uwb::protocol::fira::DeviceRole>();
        m_cliData->RangingMethodMap = populate_map<uwb::protocol::fira::RangingMethod>();
        m_cliData->MeasurementReportModeMap = populate_map<uwb::protocol::fira::MeasurementReportMode>();
        m_cliData->StsConfigurationMap = populate_map<uwb::protocol::fira::StsConfiguration>();
        m_cliData->MultiNodeModeMap = populate_map<uwb::protocol::fira::MultiNodeMode>();
        m_cliData->RangingModeMap = populate_map<uwb::protocol::fira::RangingMode>();
        m_cliData->SchedulingModeMap = populate_map<uwb::protocol::fira::SchedulingMode>();
        m_cliData->ChannelMap = populate_map<uwb::protocol::fira::Channel>();
        m_cliData->StsPacketConfigurationMap = populate_map<uwb::protocol::fira::StsPacketConfiguration>();
        m_cliData->ConvolutionalCodeConstraintLengthMap = populate_map<uwb::protocol::fira::ConvolutionalCodeConstraintLength>();
        m_cliData->PrfModeMap = populate_map<uwb::protocol::fira::PrfMode>();
        m_cliData->UwbMacAddressFcsTypeMap = populate_map<uwb::UwbMacAddressFcsType>();
    }

    auto app = std::make_unique<CLI::App>();
    app->name("nocli");
    app->description("A command line tool to assist with all things nearobject");

    app->require_subcommand();
    auto uwbApp = app->add_subcommand("uwb", "commands related to uwb")->require_subcommand()->fallthrough();

    auto rangeApp = uwbApp->add_subcommand("range", "commands related to ranging")->require_subcommand()->fallthrough();

    auto startRangingApp = rangeApp->add_subcommand("start", "start ranging")->fallthrough();

    // option to specify file
    {
        rangeApp->add_option("--file", m_cliData->defaultFile, "file to read as the default values" + m_cliData->defaultFile)->capture_default_str();
        rangeApp->callback([&]() {
            std::cout << "reading stuff from this file: " << m_cliData->defaultFile << "\n";
        });
    }

    // TODO is there a way to put all the enums into a list of [optionName, optionDestination, optionMap] so we don't have to create the initializer list each time
    // TODO get rid of these strings, instead use a macro to extract the enum name
    startRangingApp->add_option("--DeviceRole", m_cliData->sessionData.UwbConfiguration.DeviceRole)->transform(CLI::CheckedTransformer(m_cliData->DeviceRoleMap))->capture_default_str();
    startRangingApp->add_option("--RangingMethod", m_cliData->sessionData.UwbConfiguration.RangingConfiguration.Method)->transform(CLI::CheckedTransformer(m_cliData->RangingMethodMap))->capture_default_str();
    startRangingApp->add_option("--MeasurementReportMode", m_cliData->sessionData.UwbConfiguration.RangingConfiguration.ReportMode)->transform(CLI::CheckedTransformer(m_cliData->MeasurementReportModeMap))->capture_default_str();
    startRangingApp->add_option("--StsConfiguration", m_cliData->sessionData.UwbConfiguration.StsConfiguration)->transform(CLI::CheckedTransformer(m_cliData->StsConfigurationMap))->capture_default_str();
    startRangingApp->add_option("--MultiNodeMode", m_cliData->sessionData.UwbConfiguration.MultiNodeMode)->transform(CLI::CheckedTransformer(m_cliData->MultiNodeModeMap))->capture_default_str();
    startRangingApp->add_option("--RangingMode", m_cliData->sessionData.UwbConfiguration.RangingTimeStruct)->transform(CLI::CheckedTransformer(m_cliData->RangingModeMap))->capture_default_str();
    startRangingApp->add_option("--SchedulingMode", m_cliData->sessionData.UwbConfiguration.SchedulingMode)->transform(CLI::CheckedTransformer(m_cliData->SchedulingModeMap))->capture_default_str();
    startRangingApp->add_option("--Channel", m_cliData->sessionData.UwbConfiguration.Channel)->transform(CLI::CheckedTransformer(m_cliData->ChannelMap))->capture_default_str();
    startRangingApp->add_option("--StsPacketConfiguration", m_cliData->sessionData.UwbConfiguration.RFrameConfig)->transform(CLI::CheckedTransformer(m_cliData->StsPacketConfigurationMap))->capture_default_str();
    startRangingApp->add_option("--ConvolutionalCodeConstraintLength", m_cliData->sessionData.UwbConfiguration.ConvolutionalCodeConstraintLength)->transform(CLI::CheckedTransformer(m_cliData->ConvolutionalCodeConstraintLengthMap))->capture_default_str();
    startRangingApp->add_option("--PrfMode", m_cliData->sessionData.UwbConfiguration.PrfMode)->transform(CLI::CheckedTransformer(m_cliData->PrfModeMap))->capture_default_str();
    startRangingApp->add_option("--UwbMacAddressFcsType", m_cliData->sessionData.UwbConfiguration.MacAddressFcsType)->transform(CLI::CheckedTransformer(m_cliData->UwbMacAddressFcsTypeMap))->capture_default_str();

    // booleans
    startRangingApp->add_flag("--controller,!--controlee", m_cliData->hostIsController,"default is controlee")->capture_default_str();
    startRangingApp->add_flag("--HoppingMode", m_cliData->sessionData.UwbConfiguration.HoppingMode)->capture_default_str();
    startRangingApp->add_flag("--BlockStriding", m_cliData->sessionData.UwbConfiguration.BlockStriding)->capture_default_str();

    // TODO check for int sizes when parsing input
    startRangingApp->add_option("--FiraPhyVersion", m_cliData->sessionData.UwbConfiguration.FiraPhyVersion, "uint32_t")->capture_default_str();
    startRangingApp->add_option("--FiraMacVersion", m_cliData->sessionData.UwbConfiguration.FiraMacVersion, "uint32_t")->capture_default_str();
    startRangingApp->add_option("--UwbInitiationTime", m_cliData->sessionData.UwbConfiguration.UwbInitiationTime, "uint32_t")->capture_default_str();
    startRangingApp->add_option("--Sp0PhySetNumber", m_cliData->sessionData.UwbConfiguration.Sp0PhySetNumber, "uint8_t")->capture_default_str();
    startRangingApp->add_option("--Sp1PhySetNumber", m_cliData->sessionData.UwbConfiguration.Sp1PhySetNumber, "uint8_t")->capture_default_str();
    startRangingApp->add_option("--Sp3PhySetNumber", m_cliData->sessionData.UwbConfiguration.Sp3PhySetNumber, "uint8_t")->capture_default_str();
    startRangingApp->add_option("--PreableCodeIndex", m_cliData->sessionData.UwbConfiguration.PreableCodeIndex, "uint8_t")->capture_default_str();
    startRangingApp->add_option("--SlotsPerRangingRound", m_cliData->sessionData.UwbConfiguration.SlotsPerRangingRound, "uint8_t")->capture_default_str();
    startRangingApp->add_option("--MaxContentionPhaseLength", m_cliData->sessionData.UwbConfiguration.MaxContentionPhaseLength, "uint8_t")->capture_default_str();
    startRangingApp->add_option("--SlotDuration", m_cliData->sessionData.UwbConfiguration.SlotDuration, "uint8_t")->capture_default_str();
    startRangingApp->add_option("--RangingInterval", m_cliData->sessionData.UwbConfiguration.RangingInterval, "uint16_t")->capture_default_str();
    startRangingApp->add_option("--KeyRotationRate", m_cliData->sessionData.UwbConfiguration.KeyRotationRate, "uint8_t")->capture_default_str();
    startRangingApp->add_option("--MaxRangingRoundRetry", m_cliData->sessionData.UwbConfiguration.MaxRangingRoundRetry, "uint16_t")->capture_default_str();

    startRangingApp->callback([&] {
        std::cout << "Selected parameters:\n";

        for (const auto& [optionname, optionselected] :
            std::initializer_list<std::tuple<std::string_view, std::string_view>>{
                { magic_enum::enum_type_name<uwb::protocol::fira::DeviceRole>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.DeviceRole) },
                { magic_enum::enum_type_name<uwb::protocol::fira::RangingMethod>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.RangingConfiguration.Method) },
                { magic_enum::enum_type_name<uwb::protocol::fira::MeasurementReportMode>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.RangingConfiguration.ReportMode) },
                { magic_enum::enum_type_name<uwb::protocol::fira::StsConfiguration>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.StsConfiguration) },
                { magic_enum::enum_type_name<uwb::protocol::fira::MultiNodeMode>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.MultiNodeMode) },
                { magic_enum::enum_type_name<uwb::protocol::fira::RangingMode>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.RangingTimeStruct) },
                { magic_enum::enum_type_name<uwb::protocol::fira::Channel>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.Channel) },
                { magic_enum::enum_type_name<uwb::protocol::fira::StsPacketConfiguration>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.RFrameConfig) },
                { magic_enum::enum_type_name<uwb::protocol::fira::ConvolutionalCodeConstraintLength>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.ConvolutionalCodeConstraintLength) },
                { magic_enum::enum_type_name<uwb::protocol::fira::PrfMode>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.PrfMode) },
                { magic_enum::enum_type_name<uwb::UwbMacAddressFcsType>(), magic_enum::enum_name(m_cliData->sessionData.UwbConfiguration.MacAddressFcsType) } }) {
            std::cout << optionname << "::" << optionselected << "\n";
        }
    });

    auto stopRangingApp = rangeApp->add_subcommand("stop", "start ranging")->fallthrough();
    stopRangingApp->callback([&] {
        std::cout << "stop ranging\n";
    });

    return app;
}
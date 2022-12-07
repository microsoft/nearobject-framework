
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

template <typename enumtype>
std::map<std::string, enumtype>
populate_map()
{
    auto reverseMap = magic_enum::enum_entries<enumtype>();
    std::vector<std::pair<std::string, enumtype>> destvector{ reverseMap.size() };
    std::transform(std::begin(reverseMap),
        std::end(reverseMap),
        std::begin(destvector),
        [](const std::pair<enumtype, std::string_view>& input) {
            return std::pair{ std::string{ input.second }, input.first };
        });
    return { std::begin(destvector), std::end(destvector) };
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
        rangeApp->add_option("--file", m_cliData->defaultFile, "file to read as the default values, default is " + m_cliData->defaultFile);
        rangeApp->callback([&]() {
            std::cout << "reading stuff from this file: " << m_cliData->defaultFile << "\n";
        });
    }

    // TODO is there a way to put all the enums into a list of [optionName, optionDestination, optionMap] so we don't have to create the initializer list each time
    // TODO get rid of these strings, instead use a macro to extract the enum name
    startRangingApp->add_option("--DeviceRole", m_cliData->defaultConfiguration.DeviceRole)->transform(CLI::CheckedTransformer(m_cliData->DeviceRoleMap));
    startRangingApp->add_option("--RangingMethod", m_cliData->defaultConfiguration.RangingConfiguration.Method)->transform(CLI::CheckedTransformer(m_cliData->RangingMethodMap));
    startRangingApp->add_option("--MeasurementReportMode", m_cliData->defaultConfiguration.RangingConfiguration.ReportMode)->transform(CLI::CheckedTransformer(m_cliData->MeasurementReportModeMap));
    startRangingApp->add_option("--StsConfiguration", m_cliData->defaultConfiguration.StsConfiguration)->transform(CLI::CheckedTransformer(m_cliData->StsConfigurationMap));
    startRangingApp->add_option("--MultiNodeMode", m_cliData->defaultConfiguration.MultiNodeMode)->transform(CLI::CheckedTransformer(m_cliData->MultiNodeModeMap));
    startRangingApp->add_option("--RangingMode", m_cliData->defaultConfiguration.RangingTimeStruct)->transform(CLI::CheckedTransformer(m_cliData->RangingModeMap));
    startRangingApp->add_option("--SchedulingMode", m_cliData->defaultConfiguration.SchedulingMode)->transform(CLI::CheckedTransformer(m_cliData->SchedulingModeMap));
    startRangingApp->add_option("--Channel", m_cliData->defaultConfiguration.Channel)->transform(CLI::CheckedTransformer(m_cliData->ChannelMap));
    startRangingApp->add_option("--StsPacketConfiguration", m_cliData->defaultConfiguration.RFrameConfig)->transform(CLI::CheckedTransformer(m_cliData->StsPacketConfigurationMap));
    startRangingApp->add_option("--ConvolutionalCodeConstraintLength", m_cliData->defaultConfiguration.ConvolutionalCodeConstraintLength)->transform(CLI::CheckedTransformer(m_cliData->ConvolutionalCodeConstraintLengthMap));
    startRangingApp->add_option("--PrfMode", m_cliData->defaultConfiguration.PrfMode)->transform(CLI::CheckedTransformer(m_cliData->PrfModeMap));
    startRangingApp->add_option("--UwbMacAddressFcsType", m_cliData->defaultConfiguration.MacAddressFcsType)->transform(CLI::CheckedTransformer(m_cliData->UwbMacAddressFcsTypeMap));

    // booleans
    startRangingApp->add_flag("--controller", m_cliData->hostIsController, "presence of this flag indicates controller, absence means controlee");
    startRangingApp->add_flag("--HoppingMode", m_cliData->defaultConfiguration.HoppingMode, "presence of this flag indicates HoppingMode on");
    startRangingApp->add_flag("--BlockStriding", m_cliData->defaultConfiguration.BlockStriding, "presence of this flag indicates BlockStriding on");

    // TODO check for int sizes when parsing input
    startRangingApp->add_option("--FiraPhyVersion", m_cliData->defaultConfiguration.FiraPhyVersion, "uint32_t");
    startRangingApp->add_option("--FiraMacVersion", m_cliData->defaultConfiguration.FiraMacVersion, "uint32_t");
    startRangingApp->add_option("--UwbInitiationTime", m_cliData->defaultConfiguration.UwbInitiationTime, "uint32_t");
    startRangingApp->add_option("--Sp0PhySetNumber", m_cliData->defaultConfiguration.Sp0PhySetNumber, "uint8_t");
    startRangingApp->add_option("--Sp1PhySetNumber", m_cliData->defaultConfiguration.Sp1PhySetNumber, "uint8_t");
    startRangingApp->add_option("--Sp3PhySetNumber", m_cliData->defaultConfiguration.Sp3PhySetNumber, "uint8_t");
    startRangingApp->add_option("--PreableCodeIndex", m_cliData->defaultConfiguration.PreableCodeIndex, "uint8_t");
    startRangingApp->add_option("--SlotsPerRangingRound", m_cliData->defaultConfiguration.SlotsPerRangingRound, "uint8_t");
    startRangingApp->add_option("--MaxContentionPhaseLength", m_cliData->defaultConfiguration.MaxContentionPhaseLength, "uint8_t");
    startRangingApp->add_option("--SlotDuration", m_cliData->defaultConfiguration.SlotDuration, "uint8_t");
    startRangingApp->add_option("--RangingInterval", m_cliData->defaultConfiguration.RangingInterval, "uint16_t");
    startRangingApp->add_option("--KeyRotationRate", m_cliData->defaultConfiguration.KeyRotationRate, "uint8_t");
    startRangingApp->add_option("--MaxRangingRoundRetry", m_cliData->defaultConfiguration.MaxRangingRoundRetry, "uint16_t");

    startRangingApp->callback([&] {
        std::cout << "Selected parameters:\n";

        for (auto [optionname, optionselected] :
            std::initializer_list<std::tuple<std::string_view, std::string_view>>{
                { magic_enum::enum_type_name<uwb::protocol::fira::DeviceRole>(), magic_enum::enum_name(m_cliData->defaultConfiguration.DeviceRole) },
                { magic_enum::enum_type_name<uwb::protocol::fira::RangingMethod>(), magic_enum::enum_name(m_cliData->defaultConfiguration.RangingConfiguration.Method) },
                { magic_enum::enum_type_name<uwb::protocol::fira::MeasurementReportMode>(), magic_enum::enum_name(m_cliData->defaultConfiguration.RangingConfiguration.ReportMode) },
                { magic_enum::enum_type_name<uwb::protocol::fira::StsConfiguration>(), magic_enum::enum_name(m_cliData->defaultConfiguration.StsConfiguration) },
                { magic_enum::enum_type_name<uwb::protocol::fira::MultiNodeMode>(), magic_enum::enum_name(m_cliData->defaultConfiguration.MultiNodeMode) },
                { magic_enum::enum_type_name<uwb::protocol::fira::RangingMode>(), magic_enum::enum_name(m_cliData->defaultConfiguration.RangingTimeStruct) },
                { magic_enum::enum_type_name<uwb::protocol::fira::Channel>(), magic_enum::enum_name(m_cliData->defaultConfiguration.Channel) },
                { magic_enum::enum_type_name<uwb::protocol::fira::StsPacketConfiguration>(), magic_enum::enum_name(m_cliData->defaultConfiguration.RFrameConfig) },
                { magic_enum::enum_type_name<uwb::protocol::fira::ConvolutionalCodeConstraintLength>(), magic_enum::enum_name(m_cliData->defaultConfiguration.ConvolutionalCodeConstraintLength) },
                { magic_enum::enum_type_name<uwb::protocol::fira::PrfMode>(), magic_enum::enum_name(m_cliData->defaultConfiguration.PrfMode) },
                { magic_enum::enum_type_name<uwb::UwbMacAddressFcsType>(), magic_enum::enum_name(m_cliData->defaultConfiguration.MacAddressFcsType) } }) {
            std::cout << optionname << "::" << optionselected << "\n";
        }
    });

    return app;
}